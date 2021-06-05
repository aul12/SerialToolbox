/**
 * @file Interface.cpp
 * @author paul
 * @date 30.12.18
 * @brief represents an abstract serial interface
 */

#include "Interface.hpp"

namespace util::serial {
    void Interface::registerReceiveCallback(const std::function<void(std::vector<uint8_t>)> &callback) {
        if (this->callback.has_value()) {
            throw std::logic_error("You can only set one callback");
        }
        this->callback = callback;
    }

    void Interface::callbackIfAvailable(const std::vector<uint8_t> &data) {
        if (this->callback.has_value()) {
            this->callback.value()(data);
        }
    }
} // namespace util::serial
