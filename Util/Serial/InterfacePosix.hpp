/**
 * @file SerialInterface.hpp
 * @author paul
 * @date 29.12.18
 * @brief SerialInterface @TODO
 */

#ifndef SERIALTOOLBOX_INTERFACEPOSIX_HPP
#define SERIALTOOLBOX_INTERFACEPOSIX_HPP

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

        ~InterfacePosix() override;

        static auto getAvailablePorts() -> std::vector<std::string>;
    private:
        void sendBuff(const std::vector<uint8_t> &buffer) const override;
        void readerThread();
        int fd;
        std::future<void> readerThreadHandle;
        std::mutex readLock;
        static constexpr std::size_t BUF_SIZE = 4096;
    };
}

#endif //HTERMCLONE_SERIALINTERFACE_HPP
