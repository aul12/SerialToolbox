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

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#include "../Util/Serial/InterfacePosix.hpp"
#else
#error "Cannot build on Windows"
#endif

namespace controller {
    enum class Representation {
        ASCII, HEX, DEC, BIN
    };

    struct Representations {
        std::string ascii, hex, dec, bin;
    };

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
    using UsedInterface = util::serial::InterfacePosix;
#else
    using UsedInterface = util::serial::ImACompilerError
#endif


    class SerialProxy {
    public:
        explicit SerialProxy(const std::shared_ptr<util::serial::Interface> &interface);

        void send(std::vector<std::string> bytes, Representation representation);

        const util::Listener<std::deque<Representations>> receiveListener;
    private:
        void readCallback(std::vector<uint8_t>  data);

        static auto convert(std::string string, Representation representation) -> uint8_t;

        std::shared_ptr<util::serial::Interface> interface;
    };
}

#endif
