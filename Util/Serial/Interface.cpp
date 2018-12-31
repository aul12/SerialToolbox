/**
 * @file Interface.cpp
 * @author paul
 * @date 30.12.18
 * @brief Interface @TODO
 */

#include "Interface.hpp"

namespace util::serial {
    void Interface::registerReceiveCallback(const std::function<void(std::vector<uint8_t>)> &callback) {
        if (this->callback.has_value()) {
            throw std::logic_error("You can only set one callback");
        }
        this->callback = callback;
    }

    template<typename IT>
    void Interface::send(const IT &begin, const IT &end) const {
        static_assert(sizeof(decltype(*begin)) == 1, "send requires an container of byte sized objects");

        auto size = std::distance(begin, end);
        assert(size >= 0);
        std::vector<decltype(*begin)> buffer{};
        buffer.reserve(size);

        for (auto it = begin; it != end; it++) {
            buffer.push_back(*it);
        }

        writeLock.lock();
        this->send(buffer);
        writeLock.unlock();
    }
}