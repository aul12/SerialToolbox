/**
 * @file UiController.cpp
 * @author paul
 * @date 31.12.18
 * @brief UiController @TODO
 */

#include "UiController.hpp"
#include "../Util/Serial/InterfaceImplemenation.hpp"

namespace controller {
    UiController::UiController(const std::shared_ptr<view::MainView> &mainView) : mainView{mainView} {
        mainView->setPorts(util::serial::InterfaceImplementation::getAvailablePorts());

        decltype(mainView->baudSpinListener)::type baudSpinEvent = [this](int baud) {
            this->interface->setBaud(baud);
        };

        decltype(mainView->portComboListener)::type portComboEvent = [this](std::string port) {
            this->interface->setPort(port);
        };

        decltype(mainView->stopBitsSpinListener)::type stopBitsEvent = [this](int bits) {
            this->interface->setStopBits(bits);
        };

        decltype(mainView->dataBitsSpinListener)::type dataBitsEvent = [this](int bits) {
            this->interface->setDataBits(bits);
        };

        decltype(mainView->connectButtonListener)::type connectButtonEvent = [&]() {
            if (this->interface == nullptr) {
                this->interface = std::make_shared<util::serial::InterfaceImplementation>
                        (mainView->getPort(), mainView->getBaud());
                this->serialProxy = std::make_shared<SerialProxy>(this->interface);
                mainView->baudSpinListener(baudSpinEvent);
                mainView->portComboListener(portComboEvent);
                mainView->stopBitsSpinListener(stopBitsEvent);
                mainView->dataBitsSpinListener(dataBitsEvent);
            } else {
                this->interface->setPort(this->mainView->getPort());
                this->interface->setBaud(this->mainView->getBaud());
            }
        };

        mainView->connectButtonListener(connectButtonEvent);
    }
}
