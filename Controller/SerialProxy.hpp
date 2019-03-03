/**
 * @file SerialProxy.hpp
 * @author paul
 * @date 31.12.18
 * @brief Declaration of the Serial Proxy class
 */

#ifndef SERIALTOOLBOX_SERIALPROXY_HPP
#define SERIALTOOLBOX_SERIALPROXY_HPP

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
        explicit SerialProxy(const std::shared_ptr<util::serial::Interface> &interface);

        auto send(std::vector<std::string> bytes, Representation representation) -> std::deque<Representations>;

        const util::Listener<std::deque<Representations>> receiveListener;
    private:
        std::shared_ptr<util::serial::Interface> interface;
        std::map<uint8_t, std::string> specialAsciiCharacters;

        void readCallback(std::vector<uint8_t>  data);
        auto convertToByte(std::string string, Representation representation) -> uint8_t;
        auto convertToRepresentations(uint8_t data) -> Representations;


    };
}

#endif
