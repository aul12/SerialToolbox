/**
 * @file MainView.cpp
 * @author paul
 * @date 30.12.18
 * @brief MainView @TODO
 */

#include <gtkmm/builder.h>
#include "MainView.hpp"

#define WIDGET_MAP(x) #x, x

namespace view {
    MainView::MainView(const std::string &uiFile) {
        Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(uiFile);

        builder->get_widget(WIDGET_MAP(mainWindow));
        builder->get_widget(WIDGET_MAP(connectButton));
        builder->get_widget(WIDGET_MAP(portCombo));
        builder->get_widget(WIDGET_MAP(baudSpin));
        builder->get_widget(WIDGET_MAP(dataBitsSpin));
        builder->get_widget(WIDGET_MAP(stopBitsSpin));

        this->connectButton->signal_clicked().connect(sigc::mem_fun(this, &MainView::connectButtonHandler));
        this->portCombo->signal_changed().connect(sigc::mem_fun(this, &MainView::portComboHandler));
        this->baudSpin->signal_changed().connect(sigc::mem_fun(this, &MainView::baudSpinHandler));
        this->dataBitsSpin->signal_changed().connect(sigc::mem_fun(this, &MainView::dataBitsSpinHandler));
        this->stopBitsSpin->signal_changed().connect(sigc::mem_fun(this, &MainView::stopBitsSpinHandler));
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

    void MainView::connectButtonHandler() {
        if (this->connectButtonListener.has_value()) {
            this->connectButtonListener.value()();
        }
    }

    void MainView::portComboHandler() {
        if (this->portComboListener.has_value()) {
            this->portComboListener.value()(this->portCombo->get_active_text());
        }
    }

    void MainView::baudSpinHandler() {
        if (this->baudSpinListener.has_value()) {
            this->baudSpinListener.value()(this->baudSpin->get_value_as_int());
        }
    }

    void MainView::dataBitsSpinHandler() {
        if (this->dataBitsSpinListener.has_value()) {
            this->dataBitsSpinListener.value()(this->dataBitsSpin->get_value_as_int());
        }
    }

    void MainView::stopBitsSpinHandler() {
        if (this->stopBitsSpinListener.has_value()) {
            this->stopBitsSpinListener.value()(this->stopBitsSpin->get_value_as_int());
        }
    }

    void MainView::registerConnectButtonListener(std::function<void()> listener) {
        if (this->connectButtonListener.has_value()) {
            throw std::logic_error("Only one listener possible");
        }

        this->connectButtonListener = listener;
    }

    void MainView::registerPortComboListener(std::function<void(std::string)> listener) {
        if (this->portComboListener.has_value()) {
            throw std::logic_error("Only one listener possible");
        }

        this->portComboListener = listener;
    }

    void MainView::registerBaudSpinListener(std::function<void(int)> listener) {
        if (this->baudSpinListener.has_value()) {
            throw std::logic_error("Only one listener possible");
        }

        this->baudSpinListener = listener;
    }

    void MainView::registerDataBitsSpinListener(std::function<void(int)> listener) {
        if (this->dataBitsSpinListener.has_value()) {
            throw std::logic_error("Only one listener possible");
        }

        this->dataBitsSpinListener = listener;
    }

    void MainView::registerStopBitsSpinListener(std::function<void(int)> listener) {
        if (this->stopBitsSpinListener.has_value()) {
            throw std::logic_error("Only one listener possible");
        }

        this->stopBitsSpinListener = listener;
    }
}