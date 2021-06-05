/**
 * @file SerialProxy.hpp
 * @author paul
 * @date 31.12.18
 * @brief Declaration of the Serial Proxy class
 */

#ifndef SERIALTOOLBOX_SERIALPROXY_HPP
#define SERIALTOOLBOX_SERIALPROXY_HPP

#include <deque>
#include <map>
#include <memory>

#include "../Util/Listener.hpp"
#include "../Util/Serial/Interface.hpp"


namespace controller {
    enum class Representation { ASCII, HEX, DEC, BIN };

    struct Representations {
        std::string ascii, hex, dec, bin;
    };

    class SerialProxy {
      public:
        explicit SerialProxy(const std::shared_ptr<util::serial::Interface> &interface);

        auto send(const std::vector<std::string> &bytes, Representation representation) -> std::deque<Representations>;

        const util::Listener<std::deque<Representations>> receiveListener;

      private:
        std::shared_ptr<util::serial::Interface> interface;
        std::map<uint8_t, std::string> specialAsciiCharacters;

        void readCallback(const std::vector<uint8_t> &data);
        auto convertToByte(const std::string &string, Representation representation) -> uint8_t;
        auto convertToRepresentations(uint8_t data) -> Representations;
        auto specialAsciiCharactersReverseLookup(const std::string &value) const -> std::optional<uint8_t>;
    };
} // namespace controller

#endif
