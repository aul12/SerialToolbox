/**
 * @file UiController.hpp
 * @author paul
 * @date 31.12.18
 * @brief UiController @TODO
 */

#ifndef SERIALTOOLBOX_UICONTROLLER_HPP
#define SERIALTOOLBOX_UICONTROLLER_HPP

#include "SerialProxy.hpp"
#include "../View/MainView.hpp"

namespace controller {
    class UiController {
    public:
        explicit UiController(const std::shared_ptr<view::MainView> &mainView);

    private:
        std::shared_ptr<view::MainView> mainView;
        std::shared_ptr<util::serial::Interface> interface;
        std::shared_ptr<controller::SerialProxy> serialProxy;
        std::future<void> repetitionFuture;

        void baudEvent(int baud);
        void portEvent(std::string port);
        void stopBitsEvent(int stopBits);
        void dataBitsEvent(int dataBits);
        void receiveEvent(std::deque<Representations> representations);
        void sendEvent(int repr, const std::string &data, int repetitions, int period);
        void visibilityEvent(bool vis);
    };
}

#endif