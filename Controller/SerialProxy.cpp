/**
 * @file SerialProxy.cpp
 * @author paul
 * @date 31.12.18
 * @brief SerialProxy @TODO
 */

#include "SerialProxy.hpp"

namespace controller {

    SerialProxy::SerialProxy(const std::shared_ptr<util::serial::Interface> &interface)
        : interface{interface} {

    }

    void SerialProxy::registerReceiveListener(
            std::function<void(std::vector<std::unordered_map<Representation, std::string>>)> listener) {
        if (this->listener.has_value()) {
            throw std::runtime_error("There is already an listener registerd");
        }

        this->listener = listener;
    }

    void SerialProxy::send(std::vector<std::string> bytes, Representation representation) {

    }
}
