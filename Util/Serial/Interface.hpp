/**
 * @file Interface.hpp
 * @author paul
 * @date 30.12.18
 * @brief Declaration of the abstract Interface class
 */

#ifndef HTERMCLONE_INTERFACE_HPP
#define HTERMCLONE_INTERFACE_HPP

#include <string>
#include <optional>
#include <functional>
#include <vector>
#include <mutex>
#include <future>

namespace util::serial {
    enum class Parity {
        NONE, ODD, EVEN, MARK, SPACE
    };

    /**
     * Implements an OS-independent serial interface.
     */
    class Interface {
    public:
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
        void registerReceiveListenerA(const std::function<void(std::vector<uint8_t>)> &callback);
    protected:
        /**
         * Send a buffer via the serial interface, needs to be implemented by the spec
         * @param send
         */
        virtual void send(const std::vector<uint8_t> &buffer) const = 0;

        std::optional<std::function<void(std::vector<uint8_t>)>> callback;
        mutable std::mutex writeLock;
    };
}

#endif //HTERMCLONE_INTERFACE_HPP
