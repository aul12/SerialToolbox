/**
 * @file SendThread.hpp
 * @author paul
 * @date 02.03.19
 * @brief SendThread @TODO
 */

#ifndef SERIALTOOLBOX_SENDTHREAD_HPP
#define SERIALTOOLBOX_SENDTHREAD_HPP

#include <list>
#include <mutex>
#include <memory>
#include <condition_variable>
#include "../View/MainView.hpp"
#include "SerialProxy.hpp"

namespace controller {
    class SendThread {
    public:
        SendThread(std::shared_ptr<view::MainView> mainView, std::shared_ptr<controller::SerialProxy> serialProxy);

        void run();

        void send(int repr, const std::string &data, int repetitions, int period);

        ~SendThread();

    private:
        std::list<std::tuple<int, std::string, int, int>> queue;
        std::mutex queueLock;
        std::mutex dataNotify;
        std::atomic_bool finished;
        std::shared_ptr<view::MainView> mainView;
        std::shared_ptr<controller::SerialProxy> serialProxy;
        std::thread thread;
    };
}

#endif //SERIALTOOLBOX_SENDTHREAD_HPP
