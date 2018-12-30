/**
 * @file SerialInterface.cpp
 * @author paul
 * @date 29.12.18
 * @brief SerialInterface @TODO
 */

#include "Interface.hpp"

#include <fcntl.h>
#include <cstring>
#include <termios.h>
#include <unistd.h>
#include <cassert>
#include <vector>
#include <mutex>

namespace util::serial {
    template<size_t BUF_SIZE>
    Interface<BUF_SIZE>::Interface(const std::string &port, int baud, ParityOption parityOption) {
        assert(baud > 0);

        fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
        if (fd < 0) {
            throw std::runtime_error(strerror(errno));
        }

        termios tty{};
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fd, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }

        cfsetospeed(&tty, static_cast<speed_t>(baud));
        cfsetispeed(&tty, static_cast<speed_t>(baud));

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN] = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity

        tcflag_t parityFlag;
        switch (parityOption) {
            case ParityOption::NONE:
                parityFlag = 0;
                break;
            case ParityOption::ODD:
                parityFlag = PARENB | PARODD;
                break;
            case ParityOption::EVEN:
                parityFlag = PARENB;
                break;
            case ParityOption::MARK:
                parityFlag = PARENB | PARODD | CMSPAR;
                break;
            case ParityOption::SPACE:
                parityFlag = PARENB | CMSPAR;
                break;
        }
        tty.c_cflag |= parityFlag;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        tty.c_cc[VMIN] = 0;            // None blocking
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
            throw std::runtime_error(strerror(errno));
        }

        this->readerThreadHandle = std::async(std::launch::async, &Interface::readerThread, this);
    }

    template<size_t BUF_SIZE>
    template<typename IT>
    void Interface<BUF_SIZE>::send(const IT &begin, const IT &end) const {
        static_assert(sizeof(decltype(*begin)) == 1, "send requires an container of byte sized objects");

        auto size = std::distance(begin, end);
        assert(size >= 0);
        std::vector<decltype(*begin)> buffer{};
        buffer.reserve(size);

        for (auto it = begin; it != end; it++) {
            buffer.push_back(*it);
        }

        writeLock.lock();
        std::size_t written = 0;
        do {
            auto result = write(this->fd, buffer.data() + written, size - written);
            if (result < 0) {
                writeLock.unlock();
                throw std::runtime_error(strerror(errno));
            }
            written += static_cast<std::size_t>(result);
        } while (written < size);
        writeLock.unlock();
    }


    template<size_t BUF_SIZE>
    void Interface<BUF_SIZE>::registerCallback(const std::function<void(std::vector<uint8_t>)> &callback) {
        if (this->callback.has_value()) {
            throw std::logic_error("You can only set one callback");
        }
        this->callback = callback;
    }


    template<size_t BUF_SIZE>
    void Interface<BUF_SIZE>::readerThread() {
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
    Interface<BUF_SIZE>::~Interface() {
        this->readClose.lock();
        close(this->fd);
        this->readerThreadHandle.wait();
    }
}
