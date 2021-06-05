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
#include <thread>
#include "../View/MainView.hpp"
#include "SerialProxy.hpp"
#include "LineBreakStateMachine.hpp"

namespace controller {
    class SendHandler {
    public:
        SendHandler(std::shared_ptr<view::MainView> mainView, std::shared_ptr<controller::SerialProxy> serialProxy);

        void run();

        void send(int repr, const std::vector<std::string> &data, int repetitions, int period);

        void setLineBreak(LinebreakType linebreakType);

        void resetCount();

        ~SendHandler();

    private:
        std::mutex lineBreakMutex;
        LineBreakStateMachine lineBreakStateMachine;
        std::list<std::tuple<int, std::vector<std::string>, int, int>> queue;
        std::mutex queueLock;
        std::mutex dataNotify;
        std::atomic_bool finished;
        std::shared_ptr<view::MainView> mainView;
        std::shared_ptr<controller::SerialProxy> serialProxy;
        std::thread thread;
        std::atomic_int sendCount = 0;
    };
}

#endif //SERIALTOOLBOX_SENDTHREAD_HPP
