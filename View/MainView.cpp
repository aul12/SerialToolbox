/**
 * @file MainView.cpp
 * @author paul
 * @date 30.12.18
 * @brief MainView @TODO
 */

#include <gtkmm/builder.h>
#include "MainView.hpp"
#include "ByteRepresentationWidget.hpp"

#define WIDGET_MAP(x) #x, x

namespace view {
    MainView::MainView(const std::string &uiFile) {
        Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(uiFile);

        builder->get_widget(WIDGET_MAP(mainWindow));
        builder->get_widget(WIDGET_MAP(portCombo));
        builder->get_widget(WIDGET_MAP(parityCombo));
        builder->get_widget(WIDGET_MAP(baudSpin));
        builder->get_widget(WIDGET_MAP(dataBitsSpin));
        builder->get_widget(WIDGET_MAP(stopBitsSpin));
        builder->get_widget(WIDGET_MAP(checkAscii));
        builder->get_widget(WIDGET_MAP(checkHex));
        builder->get_widget(WIDGET_MAP(checkDec));
        builder->get_widget(WIDGET_MAP(checkBin));
        builder->get_widget(WIDGET_MAP(stopBitsLabel));
        builder->get_widget(WIDGET_MAP(dataBitsLabel));
        builder->get_widget(WIDGET_MAP(receiveFlow));
        builder->get_widget(WIDGET_MAP(sendFlow));
        builder->get_widget(WIDGET_MAP(encodingSendCombo));
        builder->get_widget(WIDGET_MAP(toSendEntry));
        builder->get_widget(WIDGET_MAP(repetitionsSpin));
        builder->get_widget(WIDGET_MAP(periodSpin));
        builder->get_widget(WIDGET_MAP(sendButton));
        builder->get_widget(WIDGET_MAP(sendFlow));

        this->portCombo->signal_changed().connect(sigc::mem_fun(this, &MainView::portComboHandler));
        this->baudSpin->signal_value_changed().connect(sigc::mem_fun(this, &MainView::baudSpinHandler));
        this->dataBitsSpin->signal_value_changed().connect(sigc::mem_fun(this, &MainView::dataBitsSpinHandler));
        this->stopBitsSpin->signal_value_changed().connect(sigc::mem_fun(this, &MainView::stopBitsSpinHandler));
        this->sendButton->signal_clicked().connect(sigc::mem_fun(this, &MainView::sendHandler));

        this->addReceived("A", "123", "7F", "01101100");
        this->addSend("B", "123", "7F", "01101100");

        this->representationIds.insert({"ascii", 0});
        this->representationIds.insert({"hex", 1});
        this->representationIds.insert({"dec", 2});
        this->representationIds.insert({"bin", 3});
    }

    void MainView::setPorts(const std::vector<std::string> &ports, int activeIndex) {
        this->portCombo->remove_all();
        for (const auto &port : ports) {
            this->portCombo->append(port);
        }
        this->portCombo->set_active(activeIndex);
    }

    auto MainView::getWindow() -> Gtk::Window & {
        return *(this->mainWindow);
    }

    void MainView::portComboHandler() {
        this->portComboListener(this->portCombo->get_active_text());
    }

    void MainView::baudSpinHandler() {
        baudSpinListener(this->baudSpin->get_value_as_int());
    }

    void MainView::dataBitsSpinHandler() {
        dataBitsSpinListener(this->dataBitsSpin->get_value_as_int());
    }

    void MainView::stopBitsSpinHandler() {
        stopBitsSpinListener(this->stopBitsSpin->get_value_as_int());
    }

    void MainView::sendHandler() {
        std::string repr = this->encodingSendCombo->get_active_id();
        std::string data = this->toSendEntry->get_text();
        int repe = this->repetitionsSpin->get_value_as_int();
        int inte = this->periodSpin->get_value_as_int();
        auto reprId = representationIds.find(repr);
        if (reprId == representationIds.end()) {
            throw std::runtime_error{"Invalid key!"};
        }

        sendClickListener(reprId->second, data, repe, inte);
    }

    auto MainView::getPort() const -> std::string {
        return this->portCombo->get_active_text();
    }

    auto MainView::getBaud() const -> int {
        return this->baudSpin->get_value_as_int();
    }

    auto MainView::getDataBits() const -> int {
        return this->dataBitsSpin->get_value_as_int();
    }

    auto MainView::getStopBits() const -> int {
        return this->stopBitsSpin->get_value_as_int();
    }

    auto MainView::getAsciiEnabled() const -> bool {
        return this->checkAscii->get_active();
    }

    auto MainView::getHexEnabled() const -> bool {
        return this->checkHex->get_active();
    }

    auto MainView::getDecEnabled() const -> bool {
        return this->checkDec->get_active();
    }

    auto MainView::getBinEnabled() const -> bool {
        return this->checkBin->get_active();
    }

    void MainView::setSerialOptionsVisibility(bool visible) {
        this->baudSpin->set_visible(visible);
        this->stopBitsSpin->set_visible(visible);
        this->dataBitsSpin->set_visible(visible);
        this->parityCombo->set_visible(visible);
        this->stopBitsLabel->set_visible(visible);
        this->dataBitsLabel->set_visible(visible);
    }

    void MainView::addSend(std::string ascii, std::string dec, std::string hex, std::string bin) {
        sendWidgets.emplace_back(ascii, dec, hex, bin);
        this->sendFlow->add(sendWidgets.back());
        sendWidgets.back().show();
    }

    void MainView::addReceived(std::string ascii, std::string dec, std::string hex, std::string bin) {
        receiveWidgets.emplace_back(ascii, dec, hex, bin);
        this->receiveFlow->add(receiveWidgets.back());
        receiveWidgets.back().show();
    }

    void MainView::showError(std::string title, std::string message) {
        Gtk::MessageDialog dialog{*this->mainWindow, title,
                                  false, Gtk::MESSAGE_ERROR};
        dialog.set_secondary_text(message);
        dialog.run();
    }
}