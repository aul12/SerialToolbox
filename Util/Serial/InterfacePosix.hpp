/**
 * @file SerialInterface.hpp
 * @author paul
 * @date 29.12.18
 * @brief SerialInterface @TODO
 */

#ifndef HTERMCLONE_INTERFACEPOSIX_HPP
#define HTERMCLONE_INTERFACEPOSIX_HPP

#include "Interface.hpp"

namespace util::serial {

    /**
     * Implements an Interface for Posix conforming systems (POSIX.1-2001). Tested
     * on Linux Kernel 4.15.
     * @tparam BUF_SIZE the size of the internal buffer, this is an template argument because the buffer
     * gets statically allocated.
     */
    class InterfacePosix : public Interface {
    public:
        explicit InterfacePosix(const std::string &port, int baud = 9600);

        void setBaud(int baud) override;

        void setPort(const std::string &port) override;

        void setParity(Parity parity) override;

        void setDataBits(int dataBits) override;

        void setStopBits(int stopBits) override;

        void registerCallback(const std::function<void(std::vector < uint8_t > )> &callback) override;

        ~InterfacePosix();

        static auto getAvailablePorts() -> std::vector<std::string>;
    private:
        void send(const std::vector<uint8_t> &buffer) const override;
        void readerThread();
        int fd;
        std::future<void> readerThreadHandle;
        std::mutex readClose;
        static constexpr std::size_t BUF_SIZE = 4096;
    };
}

#endif //HTERMCLONE_SERIALINTERFACE_HPP
