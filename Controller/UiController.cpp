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
            try {
                this->interface->setBaud(baud);
            } catch (std::runtime_error &e) {
                Gtk::MessageDialog dialog{this->mainView->getWindow(), "Error setting baud",
                                          false, Gtk::MESSAGE_ERROR};
                dialog.set_secondary_text(e.what());
                dialog.run();
            }
        };

        decltype(mainView->portComboListener)::type portComboEvent = [this](std::string port) {
            try {
                this->interface->setPort(port);
            } catch (std::runtime_error &e) {
                Gtk::MessageDialog dialog{this->mainView->getWindow(), "Error setting port",
                                          false, Gtk::MESSAGE_ERROR};
                dialog.set_secondary_text(e.what());
                dialog.run();
            }
        };

        decltype(mainView->stopBitsSpinListener)::type stopBitsEvent = [this](int bits) {
            try {
                this->interface->setStopBits(bits);
            } catch (std::runtime_error &e) {
                Gtk::MessageDialog dialog{this->mainView->getWindow(), "Error setting stop bits",
                                          false, Gtk::MESSAGE_ERROR};
                dialog.set_secondary_text(e.what());
                dialog.run();
            }
        };

        decltype(mainView->dataBitsSpinListener)::type dataBitsEvent = [this](int bits) {
            try {
                this->interface->setDataBits(bits);
            } catch (std::runtime_error &e) {
                Gtk::MessageDialog dialog{this->mainView->getWindow(), "Error setting data bits",
                                          false, Gtk::MESSAGE_ERROR};
                dialog.set_secondary_text(e.what());
                dialog.run();
            }
        };

        decltype(mainView->connectButtonListener)::type connectButtonEvent = [&]() {
            try {
                if (this->interface == nullptr) {
                    this->interface = std::make_shared<util::serial::InterfaceImplementation>
                            (mainView->getPort(), mainView->getBaud());
                    this->serialProxy = std::make_shared<SerialProxy>(this->interface);
                } else {
                    this->interface->setPort(this->mainView->getPort());
                    this->interface->setBaud(this->mainView->getBaud());
                }
            } catch (std::runtime_error &e) {
                Gtk::MessageDialog dialog{this->mainView->getWindow(), "Error connecting",
                                          false, Gtk::MESSAGE_ERROR};
                dialog.set_secondary_text(e.what());
                dialog.run();
            }
        };

        mainView->connectButtonListener(connectButtonEvent);

        mainView->baudSpinListener(baudSpinEvent);
        mainView->portComboListener(portComboEvent);
        mainView->stopBitsSpinListener(stopBitsEvent);
        mainView->dataBitsSpinListener(dataBitsEvent);
    }
}
