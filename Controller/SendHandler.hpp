/**
 * @file SendThread.hpp
 * @author paul
 * @date 02.03.19
 * @brief Declares a thread to send message multiple times
 */

#ifndef SERIALTOOLBOX_SENDTHREAD_HPP
#define SERIALTOOLBOX_SENDTHREAD_HPP

#include <list>
#include <mutex>
#include <memory>
#include "../View/MainView.hpp"
#include "SerialProxy.hpp"

namespace controller {
    class SendHandler {
    public:
        SendHandler(std::shared_ptr<view::MainView> mainView, std::shared_ptr<controller::SerialProxy> serialProxy);

        void run();

        void send(int repr, const std::string &data, int repetitions, int period);

        ~SendHandler();

    private:
        std::list<std::tuple<int, std::string, int, int>> queue;
        std::mutex queueLock;
        std::mutex dataNotify;
        std::mutex uiMutex;
        std::atomic_bool finished;
        std::shared_ptr<view::MainView> mainView;
        std::shared_ptr<controller::SerialProxy> serialProxy;
        std::thread thread;
    };
}

#endif //SERIALTOOLBOX_SENDTHREAD_HPP
