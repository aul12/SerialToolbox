/**
 * @file UiController.hpp
 * @author paul
 * @date 31.12.18
 * @brief Declares the main controller
 */

#ifndef SERIALTOOLBOX_UICONTROLLER_HPP
#define SERIALTOOLBOX_UICONTROLLER_HPP

#include "SerialProxy.hpp"
#include "../View/MainView.hpp"
#include "SendHandler.hpp"
#include "ConnectionContainer.hpp"
#include "LineBreakStateMachine.hpp"

namespace controller {
    class UiController {
    public:
        explicit UiController(const std::shared_ptr<view::MainView> &mainView);

    private:
        std::shared_ptr<view::MainView> mainView;
        std::optional<ConnectionContainer> connectionHandler;

        void portEvent(const std::string& port);
        void refreshEvent();
        void connectEvent();
        void baudEvent();
        void stopBitsEvent();
        void dataBitsEvent();
        void parityEvent(int sel);
        void receiveEvent(std::deque<Representations> representations);
        void sendEvent(int repr, const std::string &data, int repetitions, int period);
        void visibilityEvent(bool);
        void lineBreakEvent(int type);
        void resetRxEvent();
        void resetTxEvent();
        void clearRxEvent();
        void clearTxEvent();
        LineBreakStateMachine lineBreakStateMachine;
    };
}

#endif