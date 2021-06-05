/**
 * @file UiController.hpp
 * @author paul
 * @date 31.12.18
 * @brief Declares the main controller
 */

#ifndef SERIALTOOLBOX_UICONTROLLER_HPP
#define SERIALTOOLBOX_UICONTROLLER_HPP

#include "../View/MainView.hpp"
#include "ConnectionContainer.hpp"
#include "LineBreakStateMachine.hpp"
#include "SendHandler.hpp"
#include "SerialProxy.hpp"

namespace controller {
    class UiController {
      public:
        explicit UiController(const std::shared_ptr<view::MainView> &mainView);

      private:
        std::shared_ptr<view::MainView> mainView;
        std::optional<ConnectionContainer> connectionHandler;

        void refreshEvent();
        void connectEvent();
        void baudEvent();
        void stopBitsEvent();
        void dataBitsEvent();
        void parityEvent(int sel);
        void receiveEvent(const std::deque<Representations> &representations);
        void sendEvent(int repr, const std::string &data, int repetitions, int period);
        void visibilityEvent(bool);
        void lineBreakEvent(int type);
        void resetRxEvent();
        void resetTxEvent();
        void clearRxEvent();
        void clearTxEvent();
        LineBreakStateMachine lineBreakStateMachine;
    };
} // namespace controller

#endif
