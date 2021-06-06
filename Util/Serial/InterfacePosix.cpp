/**
 * @file SerialInterface.cpp
 * @author paul
 * @date 29.12.18
 * @brief Implementation of an interface for posix compliant systems
 */

#include "InterfacePosix.hpp"

#include <cassert>
#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <mutex>
#include <regex>
#include <termios.h>
#include <unistd.h>
#include <vector>

// NOLINTNEXTLINE
#define BAUD_TO_BITS(baud)                                                                                             \
    case baud:                                                                                                         \
        baudBits = B##baud;                                                                                            \
        break;

namespace util::serial {

    InterfacePosix::InterfacePosix(const std::string &port, int baud) : fd{0}, finished{false} {
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
        tty.c_iflag &= ~(tcflag_t) IGNBRK; // disable break processing
        tty.c_lflag = 0;                   // no signaling chars, no echo, no canonical processing
        tty.c_oflag = 0;                   // no remapping, no delays
        tty.c_cc[VMIN] = 0;                // read requires at least one character
        tty.c_cc[VTIME] = 0;               // read is  blocking
        tty.c_iflag &= ~((tcflag_t) IXON | (tcflag_t) IXOFF | (tcflag_t) IXANY); // shut off xon/xoff ctrl
        tty.c_cflag |= ((tcflag_t) CLOCAL | (tcflag_t) CREAD); // ignore modem controls, enable reading

#ifdef CRTSCTS
        tty.c_cflag &= ~(tcflag_t) CRTSCTS; // Disable hardware flow control if possible (not Posix, only Linux)
#endif

        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }

        this->readerThreadHandle = std::async(std::launch::async, &InterfacePosix::readerThread, this);
    }

    void InterfacePosix::readerThread() {
        std::array<uint8_t, BUF_SIZE> buffer{};

        while (!finished) { // @TODO maybe should use notify
            /*
             * Go fix your language (en). "read" needs to get a decent past tense form,
             * i decided on "readed" instead of the phonetically incorrect and irregular
             * "reed" (https://www.youtube.com/watch?v=A8zWWp0akUU).
             */
            auto readed = read(this->fd, buffer.data(), BUF_SIZE);

            /*
             * For some strange reasons read ignores the blocking option on the first call,
             * in this case errno is equal to EAGAIN (11).
             */
            if (readed < 0 && errno != EAGAIN) {
                throw std::runtime_error(strerror(errno));
            }

            if (readed > 0) {
                this->callbackIfAvailable({buffer.data(), buffer.data() + readed});
            }
        }
    }

    InterfacePosix::~InterfacePosix() {
        finished = true;
        this->readerThreadHandle.wait();
        close(this->fd);
    }

    void InterfacePosix::sendBuff(const std::vector<uint8_t> &buffer) const {
        std::size_t written = 0;
        do {
            auto result = write(this->fd, &buffer[written], buffer.size() - written);
            if (result < 0) {
                throw std::runtime_error(strerror(errno));
            }
            written += static_cast<std::size_t>(result);
        } while (written < buffer.size());
    }


    void InterfacePosix::setBaud(int baud) {
        speed_t baudBits = B9600;
        switch (baud) {
            BAUD_TO_BITS(50)
            BAUD_TO_BITS(75)
            BAUD_TO_BITS(110)
            BAUD_TO_BITS(134)
            BAUD_TO_BITS(150)
            BAUD_TO_BITS(200)
            BAUD_TO_BITS(300)
            BAUD_TO_BITS(600)
            BAUD_TO_BITS(1200)
            BAUD_TO_BITS(1800)
            BAUD_TO_BITS(2400)
            BAUD_TO_BITS(4800)
            BAUD_TO_BITS(9600)
            BAUD_TO_BITS(19200)
            BAUD_TO_BITS(38400)
#ifdef B57600 // Not a part of POSIX.1, but supported by Linux 4.15
            BAUD_TO_BITS(57600)
            BAUD_TO_BITS(115200)
            BAUD_TO_BITS(230400)
            BAUD_TO_BITS(460800)
            BAUD_TO_BITS(500000)
            BAUD_TO_BITS(576000)
            BAUD_TO_BITS(921600)
            BAUD_TO_BITS(1000000)
            BAUD_TO_BITS(1152000)
            BAUD_TO_BITS(1500000)
            BAUD_TO_BITS(2000000)
            BAUD_TO_BITS(2500000)
            BAUD_TO_BITS(3000000)
            BAUD_TO_BITS(3500000)
            BAUD_TO_BITS(4000000)
#endif
            default:
                throw std::runtime_error("Not a valid baud");
        }
        termios tty{};
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fd, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }
        cfsetospeed(&tty, static_cast<speed_t>(baudBits));
        cfsetispeed(&tty, static_cast<speed_t>(baudBits));

        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }
    }

    void InterfacePosix::setPort(const std::string &port) {
        if (this->fd >= 0) {
            close(this->fd);
        }

        fd = open(port.c_str(), (unsigned) O_RDWR | (unsigned) O_NOCTTY | (unsigned) O_SYNC);
        if (fd < 0) {
            throw std::runtime_error(strerror(errno));
        }
    }

    void InterfacePosix::setParity(Parity parity) {
        termios tty{};
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fd, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }

        tty.c_cflag &= ~((tcflag_t) PARENB | (tcflag_t) PARODD); // shut off parity

        tcflag_t parityFlag = 0;
        switch (parity) {
            case Parity::NONE:
                parityFlag = 0;
                break;
            case Parity::ODD:
                parityFlag = (tcflag_t) PARENB | (tcflag_t) PARODD;
                break;
            case Parity::EVEN:
                parityFlag = PARENB;
                break;
            case Parity::MARK:
                parityFlag = (tcflag_t) PARENB | (tcflag_t) PARODD | (tcflag_t) CMSPAR;
                break;
            case Parity::SPACE:
                parityFlag = (tcflag_t) PARENB | (tcflag_t) CMSPAR;
                break;
        }
        tty.c_cflag |= parityFlag;

        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }
    }

    void InterfacePosix::setDataBits(int dataBits) {

        termios tty{};
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fd, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }

        tcflag_t charSize = CS8;
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
        tty.c_cflag = (tty.c_cflag & ~((tcflag_t) CSIZE)) | charSize; // 8-bit chars

        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }
    }

    void InterfacePosix::setStopBits(int stopBits) {
        termios tty{};
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fd, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }

        if (stopBits == 1) {
            tty.c_cflag &= ~((tcflag_t) CSTOPB);
        } else if (stopBits == 2) {
            tty.c_cflag |= (tcflag_t) CSTOPB;
        } else {
            throw std::runtime_error("stopBits needs to be 1 or 2");
        }

        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }
    }

    auto InterfacePosix::getAvailablePorts() -> std::vector<std::string> {
        std::string path = "/dev/";
        std::regex deviceRegex{"tty[a-zA-Z]+[0-9]+"};
        std::vector<std::string> results;
        for (const auto &entry : std::filesystem::directory_iterator(path)) {
            if (entry.is_character_file()) {
                std::string fileName = entry.path().filename();
                if (std::regex_match(fileName, deviceRegex)) {
                    results.emplace_back(entry.path());
                }
            }
        }
        return results;
    }
} // namespace util::serial
