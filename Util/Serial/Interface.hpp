/**
 * @file Interface.hpp
 * @author paul
 * @date 30.12.18
 * @brief Declaration of the abstract Interface class
 */

#ifndef SERIALTOOLBOX_INTERFACE_HPP
#define SERIALTOOLBOX_INTERFACE_HPP

#include <cassert>
#include <cstdint>
#include <functional>
#include <future>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

namespace util::serial {
    enum class Parity { NONE = 0, EVEN = 1, ODD = 2, SPACE = 3, MARK = 4 };

    /**
     * Implements an OS-independent serial interface.
     */
    class Interface {
      public:
        Interface() = default;

        Interface(const Interface &) = delete;

        auto operator=(const Interface &) = delete;

        Interface(Interface &&) = delete;

        auto operator=(Interface &&) = delete;

        /**
         * Set the Baud rate.
         * @param baud the baud rate
         * @throws runtime_error if the baud is invalid (depending on the platform)
         */
        virtual void setBaud(int baud) = 0;

        /**
         * Set the Port id (eg. /dev/ttyACM0 or COM1). If there is already an connection this connection
         * needs to be closed.
         * @param port the string represenation of the port
         * @throws runtime_error if the file does not exist
         */
        virtual void setPort(const std::string &port) = 0;

        /**
         * Set the parity option.
         * @param parity the way the parity is calculated (if any).
         * @throws runtime_error if this option is not possible with this interface.
         */
        virtual void setParity(Parity parity) = 0;

        /**
         * Set the word length.
         * @param dataBits almost always between 7 and 10. Most of the time 8
         * @throws runtime_error if this option is not possible with this interface.
         */
        virtual void setDataBits(int dataBits) = 0;

        /**
         * Set the number of stop bits.
         * @param stopBits the number of stop bits
         * @throws runtime_error if this option is not possible with this interface.
         */
        virtual void setStopBits(int stopBits) = 0;

        /**
         * Send content of an generic container
         * @tparam IT an iterator or iterator like interface
         * @param begin iterator to the first item of the container
         * @param end iterator to the last item of the container
         */
        template<typename IT>
        void send(const IT &begin, const IT &end) const;

        /**
         * Register an callback which gets called when new data is available
         * @param callback
         */
        void registerReceiveCallback(const std::function<void(std::vector<uint8_t>)> &callback);

        /**
         * Default destructor, necessary when destructing a derived class from a base pointer.
         */
        virtual ~Interface() = default;

      protected:
        /**
         * Send a buffer via the serial interface, needs to be implemented by the spec
         * @param send
         */
        virtual void sendBuff(const std::vector<uint8_t> &buffer) const = 0;

        /**
         * Checks if a callback is set and calls it if available
         * @param data the data to send to the callback
         */
        void callbackIfAvailable(const std::vector<uint8_t> &data);

      private:
        std::optional<std::function<void(const std::vector<uint8_t> &)>> callback;
        mutable std::mutex writeLock;
    };

    template<typename IT>
    void Interface::send(const IT &begin, const IT &end) const {
        using T = typename std::iterator_traits<IT>::value_type;
        static_assert(sizeof(T) == 1, "send requires an container of byte sized objects");

        auto size = std::distance(begin, end);
        if (size < 0) {
            throw std::runtime_error("Invalid size");
        }

        if (size == 0) {
            return;
        }

        std::vector<uint8_t> buffer{};
        buffer.reserve(static_cast<unsigned long>(size));

        for (auto it = begin; it != end; it++) {
            buffer.push_back(static_cast<uint8_t>(*it));
        }

        std::lock_guard guard{writeLock};
        this->sendBuff(buffer);
    }
} // namespace util::serial

#endif
