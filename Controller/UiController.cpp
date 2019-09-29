/**
 * @file UiController.cpp
 * @author paul
 * @date 31.12.18
 * @brief Implements the main controller
 */

#include "UiController.hpp"
#include "../Util/Serial/InterfaceImplemenation.hpp"

#include <functional>

namespace controller {
    UiController::UiController(const std::shared_ptr<view::MainView> &mainView) : mainView{mainView},
        lineBreakStateMachine{static_cast<LinebreakType>(mainView->getLinebreak())} {

        mainView->setPorts(util::serial::InterfaceImplementation::getAvailablePorts(), -1);

        std::function<void()> refreshBind = std::bind(&UiController::refreshEvent, this);
        std::function<void()> connectBind = std::bind(&UiController::connectEvent, this);
        std::function<void()> baudBind = std::bind(&UiController::baudEvent, this);
        std::function<void()> stopBitBind = std::bind(&UiController::stopBitsEvent, this);
        std::function<void()> dataBitsBind = std::bind(&UiController::dataBitsEvent, this);
        std::function<void(int)> parityBind = std::bind(&UiController::parityEvent, this, std::placeholders::_1);
        std::function<void(int,const std::string&,int,int)> sendBind =
                std::bind(&UiController::sendEvent, this, std::placeholders::_1, std::placeholders::_2,
                        std::placeholders::_3, std::placeholders::_4);
        std::function<void(bool)> visibilityBind = std::bind(&UiController::visibilityEvent, this, std::placeholders::_1);
        std::function<void(int)> lineBreakBind = std::bind(&UiController::lineBreakEvent, this, std::placeholders::_1);
        std::function<void()> rxResetBind = std::bind(&UiController::resetRxEvent, this);
        std::function<void()> txResetBind = std::bind(&UiController::resetTxEvent, this);
        std::function<void()> rxClearBind = std::bind(&UiController::clearRxEvent, this);
        std::function<void()> txClearBind = std::bind(&UiController::clearTxEvent, this);

        mainView->refreshListener(refreshBind);
        mainView->connectListener(connectBind);
        mainView->baudSpinListener(baudBind);
        mainView->stopBitsSpinListener(stopBitBind);
        mainView->dataBitsSpinListener(dataBitsBind);
        mainView->parityListener(parityBind);
        mainView->sendClickListener(sendBind);
        mainView->hexEnabledListener(visibilityBind);
        mainView->decEnabledListener(visibilityBind);
        mainView->binEnabledListener(visibilityBind);
        mainView->asciiEnabledListener(visibilityBind);
        mainView->linebreakListener(lineBreakBind);
        mainView->resetTxListener(txResetBind);
        mainView->resetRxListener(rxResetBind);
        mainView->clearRxListener(rxClearBind);
        mainView->clearTxListener(txClearBind);
    }

    void UiController::refreshEvent() {
        auto currPort = mainView->getPort();
        auto ports = util::serial::InterfaceImplementation::getAvailablePorts();
        auto activeIndex = -1;

        for (std::size_t c=0; c<ports.size(); ++c) {
            if (ports[c] == currPort) {
                activeIndex = static_cast<int>(c);
            }
        }

        mainView->setPorts(ports, activeIndex);
    }

    void UiController::connectEvent() {
        if (!this->connectionHandler.has_value()) {
            try {
                this->connectionHandler.reset();

                ConnectionContainer nConnHandler;
                nConnHandler.interface = std::make_shared<util::serial::InterfaceImplementation>
                        (mainView->getPort(), this->mainView->getBaud());
                nConnHandler.serialProxy = std::make_shared<SerialProxy>(nConnHandler.interface);
                nConnHandler.sendThread = std::make_shared<SendHandler>(this->mainView, nConnHandler.serialProxy);

                std::function<void(std::deque<Representations>)> receiveBind =
                        std::bind(&UiController::receiveEvent, this, std::placeholders::_1);
                nConnHandler.serialProxy->receiveListener(receiveBind);
                this->connectionHandler = nConnHandler;

                this->mainView->setConnectButtonText("Disconnect");
            } catch (std::runtime_error &e) {
                this->mainView->showError("Error setting port", e.what());
                this->connectionHandler.reset();
                this->mainView->setConnectButtonText("Connect");
            }
        } else {
            this->connectionHandler.reset();
            this->mainView->setConnectButtonText("Connect");
        }
    }

    void UiController::baudEvent() {
        try {
            if (this->connectionHandler.has_value()) {
                this->connectionHandler.value().interface->setBaud(this->mainView->getBaud());
            }
        } catch (std::runtime_error &e) {
            this->mainView->showError("Error setting baud", e.what());
        }
    }

    void UiController::stopBitsEvent() {
        try {
            if (this->connectionHandler.has_value()) {
                this->connectionHandler.value().interface->setStopBits(this->mainView->getStopBits());
            }
        } catch (std::runtime_error &e) {
            this->mainView->showError("Error setting stop bits", e.what());
        }
    }

    void UiController::dataBitsEvent() {
        try {
            if (this->connectionHandler.has_value()) {
                this->connectionHandler.value().interface->setDataBits(this->mainView->getDataBits());
            }
        } catch (std::runtime_error &e) {
            this->mainView->showError("Error setting data bits", e.what());
        }
    }

    void UiController::receiveEvent(const std::deque<Representations>& representations) {
        for (const auto &repr : representations) {
            this->mainView->addReceived(repr.ascii, repr.dec, repr.hex, repr.bin,
                    lineBreakStateMachine.addAscii(repr.ascii));
        }
        this->connectionHandler.value().received += representations.size();
        this->mainView->setRxCount(this->connectionHandler.value().received);
    }

    void UiController::sendEvent(int repr, const std::string &data, int repetitions, int period) {
        if (this->connectionHandler.has_value()) {
            std::vector<std::string> tokens{};
            size_t pos = 0;
            auto text = data;
            while ((pos = text.find(' ')) != std::string::npos) {
                auto token = text.substr(0, pos);
                if (!token.empty()) {
                    tokens.emplace_back(std::move(token));
                }
                text.erase(0, pos + 1);
            }
            tokens.emplace_back(std::move(text));

            this->connectionHandler.value().sendThread->send(repr, tokens, repetitions, period);
        }
    }

    void UiController::visibilityEvent(bool) {
        this->mainView->setVisibility(mainView->getAsciiEnabled(),
                mainView->getDecEnabled(), mainView->getHexEnabled(), mainView->getBinEnabled());
    }

    void UiController::lineBreakEvent(int type) {
        this->lineBreakStateMachine.setLinebreak(static_cast<LinebreakType>(type));
        if (this->connectionHandler.has_value()) {
            this->connectionHandler->sendThread->setLineBreak(static_cast<LinebreakType>(type));
        }
    }

    void UiController::resetRxEvent() {
        if (this->connectionHandler.has_value()) {
            this->connectionHandler.value().received = 0;
            this->mainView->setRxCount(0);
        }
    }

    void UiController::resetTxEvent() {
        if (this->connectionHandler.has_value()) {
            this->connectionHandler.value().sendThread->resetCount();
            this->mainView->setTxCount(0);
        }
    }

    void UiController::clearRxEvent() {
        this->mainView->clearReceived();
    }

    void UiController::clearTxEvent() {
        this->mainView->clearSent();
    }

    void UiController::parityEvent(int sel) {
        if (this->connectionHandler.has_value()) {
            this->connectionHandler.value().interface->setParity(static_cast<util::serial::Parity>(sel));
        }
    }
}
