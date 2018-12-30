/**
 * @file SerialInterface.cpp
 * @author paul
 * @date 29.12.18
 * @brief SerialInterface @TODO
 */

#include "InterfacePosix.hpp"

#include <fcntl.h>
#include <cstring>
#include <termios.h>
#include <unistd.h>
#include <cassert>
#include <vector>
#include <mutex>

namespace util::serial {
    template<size_t BUF_SIZE>
    InterfacePosix<BUF_SIZE>::InterfacePosix(const std::string &port, int baud) {
        this->setPort(port);
        this->setBaud(baud);
        this->setParity(Parity::NONE);
        this->setDataBits(8);
        this->setStopBits(1);

        termios tty{};
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fd, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }

        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo, no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN] = 0;            // read doesn't block
        tty.c_cc[VTIME] = 1;            // 0.1 seconds read timeout
        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl
        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls, enable reading

#ifdef CRTSCTS
        tty.c_cflag &= ~CRTSCTS; // Disable hardware flow control if possible (not Posix, only Linux)
#endif

        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }

    }

    template<size_t BUF_SIZE>
    void InterfacePosix<BUF_SIZE>::readerThread() {
        std::array<uint8_t, BUF_SIZE> buffer;

        while (this->readClose.try_lock()) { // @TODO maybe should use notify
            this->readClose.unlock();

            auto readed = read(this->fd, buffer.data(), BUF_SIZE);

            if (readed < 0) {
                throw std::runtime_error(strerror(errno));
            } else if (readed > 0) {
                this->callback({buffer, buffer + readed});
            }
        }
    }

    template<size_t BUF_SIZE>
    InterfacePosix<BUF_SIZE>::~InterfacePosix() {
        this->readClose.lock();
        close(this->fd);
        this->readerThreadHandle.wait();
    }

    template<size_t BUF_SIZE>
    void InterfacePosix<BUF_SIZE>::send(const std::vector<uint8_t> &buffer) const {
        std::size_t written = 0;
        do {
            auto result = write(this->fd, buffer.data() + written, buffer.size() - written);
            if (result < 0) {
                writeLock.unlock();
                throw std::runtime_error(strerror(errno));
            }
            written += static_cast<std::size_t>(result);
        } while (written < buffer.size());
    }

    template<size_t BUF_SIZE>
    void InterfacePosix<BUF_SIZE>::registerCallback(const std::function<void(std::vector<uint8_t>)> &callback) {
        Interface::registerCallback(callback);
        this->readerThreadHandle = std::async(std::launch::async, &InterfacePosix::readerThread, this);
    }

    template<size_t BUF_SIZE>
    void InterfacePosix<BUF_SIZE>::setBaud(int baud) {
        termios tty{};
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fd, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }
        cfsetospeed(&tty, static_cast<speed_t>(baud));
        cfsetispeed(&tty, static_cast<speed_t>(baud));

        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }
    }

    template<size_t BUF_SIZE>
    void InterfacePosix<BUF_SIZE>::setPort(const std::string &port) {
        if (this->fd >= 0) {
            close(this->fd);
        }

        fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
        if (fd < 0) {
            throw std::runtime_error(strerror(errno));
        }
    }

    template<size_t BUF_SIZE>
    void InterfacePosix<BUF_SIZE>::setParity(Parity parity) {
        termios tty{};
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fd, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }

        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity

        tcflag_t parityFlag;
        switch (parity) {
            case Parity::NONE:
                parityFlag = 0;
                break;
            case Parity::ODD:
                parityFlag = PARENB | PARODD;
                break;
            case Parity::EVEN:
                parityFlag = PARENB;
                break;
            case Parity::MARK:
                parityFlag = PARENB | PARODD | CMSPAR;
                break;
            case Parity::SPACE:
                parityFlag = PARENB | CMSPAR;
                break;
        }
        tty.c_cflag |= parityFlag;

        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }
    }

    template<size_t BUF_SIZE>
    void InterfacePosix<BUF_SIZE>::setDataBits(int dataBits) {

        termios tty{};
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fd, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }

        tcflag_t charSize;
        switch (dataBits) {
            case 5:
                charSize = CS5;
                break;
            case 6:
                charSize = CS6;
                break;
            case 7:
                charSize = CS7;
                break;
            case 8:
                charSize = CS8;
                break;
            default:
                throw std::runtime_error("Invalid dataBits (needs to be \\in [5,8]");
        }
        tty.c_cflag = (tty.c_cflag & ~CSIZE) | charSize;     // 8-bit chars

        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }

    }

    template<size_t BUF_SIZE>
    void InterfacePosix<BUF_SIZE>::setStopBits(int stopBits) {
        termios tty{};
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fd, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }

        if (stopBits == 1) {
            tty.c_cflag &= ~CSTOPB;
        } else if (stopBits == 2) {
            tty.c_cflag |= CSTOPB;
        } else {
            throw std::runtime_error("stopBits needs to be 1 or 2");
        }

        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }
    }
}
