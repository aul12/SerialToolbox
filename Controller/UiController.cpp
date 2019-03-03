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
    UiController::UiController(const std::shared_ptr<view::MainView> &mainView) : mainView{mainView} {
        mainView->setPorts(util::serial::InterfaceImplementation::getAvailablePorts(), -1);

        std::function<void(int)> baudBind = std::bind(&UiController::baudEvent, this, std::placeholders::_1);
        std::function<void(std::string)> portBind = std::bind(&UiController::portEvent, this, std::placeholders::_1);
        std::function<void(int)> stopBitBind = std::bind(&UiController::stopBitsEvent, this, std::placeholders::_1);
        std::function<void(int)> dataBitsBind = std::bind(&UiController::dataBitsEvent, this, std::placeholders::_1);
        std::function<void(int,const std::string&,int,int)> sendBind =
                std::bind(&UiController::sendEvent, this, std::placeholders::_1, std::placeholders::_2,
                        std::placeholders::_3, std::placeholders::_4);
        std::function<void(bool)> visibilityBind = std::bind(&UiController::visibilityEvent, this, std::placeholders::_1);
        mainView->baudSpinListener(baudBind);
        mainView->portComboListener(portBind);
        mainView->stopBitsSpinListener(stopBitBind);
        mainView->dataBitsSpinListener(dataBitsBind);
        mainView->sendClickListener(sendBind);
        mainView->hexEnabledListener(visibilityBind);
        mainView->decEnabledListener(visibilityBind);
        mainView->binEnabledListener(visibilityBind);
        mainView->asciiEnabledListener(visibilityBind);
    }

    void UiController::baudEvent(int baud) {
        try {
            if (this->connectionHandler.has_value()) {
                this->connectionHandler.value().interface->setBaud(baud);
            }
        } catch (std::runtime_error &e) {
            this->mainView->showError("Error setting baud", e.what());
        }
    }

    void UiController::portEvent(std::string port) {
        try {
            this->connectionHandler.reset();

            ConnectionContainer nConnHandler;
            nConnHandler.interface = std::make_shared<util::serial::InterfaceImplementation>
                    (port, this->mainView->getBaud());
            nConnHandler.serialProxy = std::make_shared<SerialProxy>(nConnHandler.interface);
            nConnHandler.sendThread = std::make_shared<SendThread>(this->mainView, nConnHandler.serialProxy);

            std::function<void(std::deque<Representations>)> receiveBind =
                    std::bind(&UiController::receiveEvent, this, std::placeholders::_1);
            nConnHandler.serialProxy->receiveListener(receiveBind);
            this->connectionHandler = nConnHandler;

            this->mainView->setSerialOptionsVisibility(true);
        } catch (std::runtime_error &e) {
            this->mainView->showError("Error setting port", e.what());
            this->mainView->setSerialOptionsVisibility(false);
        }
    }

    void UiController::stopBitsEvent(int stopBits) {
        try {
            if (this->connectionHandler.has_value()) {
                this->connectionHandler.value().interface->setStopBits(stopBits);
            }
        } catch (std::runtime_error &e) {
            this->mainView->showError("Error setting stop bits", e.what());
        }
    }

    void UiController::dataBitsEvent(int dataBits) {
        try {
            if (this->connectionHandler.has_value()) {
                this->connectionHandler.value().interface->setDataBits(dataBits);
            }
        } catch (std::runtime_error &e) {
            this->mainView->showError("Error setting data bits", e.what());
        }
    }

    void UiController::receiveEvent(std::deque<Representations> representations) {
        for (const auto &repr : representations) {
            this->mainView->addReceived(repr.ascii, repr.dec, repr.hex, repr.bin);
        }
    }

    void UiController::sendEvent(int repr, const std::string &data, int repetitions, int period) {
        if (this->connectionHandler.has_value()) {
            this->connectionHandler.value().sendThread->send(repr, data, repetitions, period);
        }
    }

    void UiController::visibilityEvent(bool vis) {
        this->mainView->setVisibility(mainView->getAsciiEnabled(),
                mainView->getDecEnabled(), mainView->getHexEnabled(), mainView->getBinEnabled());
    }
}
