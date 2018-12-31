/**
 * @file SerialProxy.hpp
 * @author paul
 * @date 31.12.18
 * @brief SerialProxy @TODO
 */

#ifndef HTERMCLONE_SERIALPROXY_HPP
#define HTERMCLONE_SERIALPROXY_HPP

#include <memory>
#include <map>
#include <deque>
#include "../Util/Serial/Interface.hpp"
#include "../Util/Listener.hpp"

namespace controller {
    enum class Representation {
        ASCII, HEX, DEC, BIN
    };

    struct Representations {
        std::string ascii, hex, dec, bin;
    };

    class SerialProxy {
    public:
        SerialProxy(const std::shared_ptr<util::serial::Interface> &interface);

        void send(std::vector<std::string> bytes, Representation representation);

        const util::Listener<std::deque<Representations>> receiveListener;
    private:
        void readCallback(std::vector<uint8_t>  data);

        std::shared_ptr<util::serial::Interface> interface;
    };
}

#endif
