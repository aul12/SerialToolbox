/**
 * @file InterfaceImpl.hpp
 * @author paul
 * @date 01.01.19
 */

#ifndef HTERMCLONE_INTERFACEIMPL_HPP
#define HTERMCLONE_INTERFACEIMPL_HPP

#include "Util/Serial/Interface.hpp"

class InterfaceMock : public util::serial::Interface {
  public:
    void setBaud(int baud) override {
        this->baud = baud;
    }

    void setPort(const std::string &port) override {
        this->port = port;
    }

    void setParity(util::serial::Parity parity) override {
        this->parity = parity;
    }

    void setDataBits(int dataBits) override {
        this->dataBits = dataBits;
    }

    void setStopBits(int stopBits) override {
        this->stopBits = stopBits;
    }

    void doCallback(const std::vector<uint8_t> &data) {
        this->callbackIfAvailable(data);
    }

    int baud, dataBits, stopBits;
    std::string port;
    util::serial::Parity parity;
    mutable std::vector<uint8_t> buffer;

  protected:
    void sendBuff(const std::vector<uint8_t> &buffer) const override {
        this->buffer = buffer;
    }
};

#endif // HTERMCLONE_INTERFACEIMPL_HPP
