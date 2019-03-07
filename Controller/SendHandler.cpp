#include <utility>

/**
 * @file SendThread.cpp
 * @author paul
 * @date 02.03.19
 * @brief Implementation of a multithreaded and thread save sender
 */

#include "SendHandler.hpp"

namespace controller {
    SendHandler::SendHandler(std::shared_ptr<view::MainView> mainView,
                           std::shared_ptr<controller::SerialProxy> serialProxy)
            : lineBreakStateMachine{static_cast<LinebreakType>(mainView->getLinebreak())},
                finished{false}, mainView{std::move(mainView)}, serialProxy{std::move(serialProxy)},
                thread{&SendHandler::run, this} {
    }

    void SendHandler::run() {
        while (!finished) {
            queueLock.lock();
            if (queue.empty()) {
                queueLock.unlock();
                dataNotify.lock();
            } else {
                auto elem = queue.back();
                queue.pop_back();
                queueLock.unlock();

                for (auto c = 0; c < std::get<2>(elem); c++) {
                    try {
                        auto res = this->serialProxy->send({std::get<1>(elem)},
                                                           static_cast<Representation>(std::get<0>(elem)));
                        lineBreakMutex.lock();
                        this->mainView->addSend(res.front().ascii, res.front().dec,
                                                res.front().hex, res.front().bin,
                                                lineBreakStateMachine.addAscii(res.front().ascii));
                        lineBreakMutex.unlock();
                    } catch (std::runtime_error &e) {
                        this->mainView->showError("Error sending", e.what());
                    }

                    if (c + 1 < std::get<2>(elem)) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(std::get<3>(elem)));
                    }
                }
            }
        }
    }

    void SendHandler::send(int repr, const std::string &data, int repetitions, int period) {
        std::lock_guard<std::mutex> l{queueLock};
        queue.emplace_back(repr, data, repetitions, period);
        dataNotify.unlock();
    }

    SendHandler::~SendHandler() {
        finished = true;
        dataNotify.unlock();
    }

    void SendHandler::setLineBreak(LinebreakType linebreakType) {
        lineBreakMutex.lock();
        this->lineBreakStateMachine.setLinebreak(linebreakType);
        lineBreakMutex.unlock();
    }
}
