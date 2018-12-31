/**
 * @file SerialProxy.hpp
 * @author paul
 * @date 31.12.18
 * @brief SerialProxy @TODO
 */

#ifndef HTERMCLONE_SERIALPROXY_HPP
#define HTERMCLONE_SERIALPROXY_HPP

#include <memory>
#include <unordered_map>
#include "../Util/Serial/Interface.hpp"

namespace controller {
    enum class Representation {
        ASCII, HEX, DEC, BIN
    };

    class SerialProxy {
    public:
        SerialProxy(const std::shared_ptr<util::serial::Interface> &interface);

        void registerReceiveListener(
                std::function<void(std::vector<std::unordered_map<Representation, std::string>>)> listener);

        void send(std::vector<std::string> bytes, Representation representation);
    private:

        std::optional<std::function<void(std::vector<std::unordered_map<Representation, std::string>>)>> listener;
        std::shared_ptr<util::serial::Interface> interface;
    };
}

#endif
