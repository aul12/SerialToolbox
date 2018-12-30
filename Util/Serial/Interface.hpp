/**
 * @file SerialInterface.hpp
 * @author paul
 * @date 29.12.18
 * @brief SerialInterface @TODO
 */

#ifndef HTERMCLONE_SERIALINTERFACE_HPP
#define HTERMCLONE_SERIALINTERFACE_HPP


#include <string>
#include <optional>
#include <functional>
#include <vector>
#include <mutex>
#include <future>

namespace util::serial {
    enum class ParityOption {
        NONE, ODD, EVEN, MARK, SPACE
    };

    template<std::size_t BUF_SIZE = 4096>
    class Interface {
    public:
        explicit Interface(const std::string &port, int baud = 9600,
                                 ParityOption parityOption = ParityOption::NONE);

        template<typename IT>
        void send(const IT &begin, const IT &end) const;

        void registerCallback(const std::function<void(std::vector<uint8_t>)> &callback);

        ~Interface();

    private:
        void readerThread();

        int fd;
        std::optional<std::function<void(std::vector<uint8_t>)>> callback;
        std::future<void> readerThreadHandle;
        mutable std::mutex writeLock;
        std::mutex readClose;
    };
}

#endif //HTERMCLONE_SERIALINTERFACE_HPP
