/**
 * @file Listener.hpp
 * @author paul
 * @date 31.12.18
 * @brief Implements a simple generic listener with multiple receivers
 */

#ifndef SERIALTOOLBOX_LISTENER_HPP
#define SERIALTOOLBOX_LISTENER_HPP

#include <list>
#include <functional>

namespace util {
    template<typename ...Args>
    class Listener {
    public:
        Listener() = default;

        void operator()(std::function<void(Args...)> &listener) const;

        void operator()(Args... args) const;

        auto getSubscribed() const -> std::size_t;

        using type = std::function<void(Args...)>;
    private:
        mutable std::list<std::function<void(Args...)>> listeners;
    };

    template<typename... Args>
    void Listener<Args...>::operator()(std::function<void(Args...)> &listener) const {
        this->listeners.push_back(listener);
    }

    template<typename... Args>
    void Listener<Args...>::operator()(Args... args) const {
        for (const auto &listener : this->listeners) {
            listener.operator()(args...);
        }
    }

    template<typename... Args>
    auto Listener<Args...>::getSubscribed() const -> std::size_t {
        return this->listeners.size();
    }
}

#endif //SERIALTOOLBOX_LISTENER_HPP
