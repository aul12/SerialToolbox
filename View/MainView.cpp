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
    MainView::MainView(const std::string &uiFile) : connectButtonListener{} {
        Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(uiFile);

        builder->get_widget(WIDGET_MAP(mainWindow));
        builder->get_widget(WIDGET_MAP(connectButton));
        builder->get_widget(WIDGET_MAP(portCombo));
        builder->get_widget(WIDGET_MAP(baudSpin));
        builder->get_widget(WIDGET_MAP(dataBitsSpin));
        builder->get_widget(WIDGET_MAP(stopBitsSpin));

        this->connectButton->signal_clicked().connect(sigc::mem_fun(this, &MainView::connectButtonHandler));
        this->portCombo->signal_changed().connect(sigc::mem_fun(this, &MainView::portComboHandler));
        this->baudSpin->signal_value_changed().connect(sigc::mem_fun(this, &MainView::baudSpinHandler));
        this->dataBitsSpin->signal_value_changed().connect(sigc::mem_fun(this, &MainView::dataBitsSpinHandler));
        this->stopBitsSpin->signal_value_changed().connect(sigc::mem_fun(this, &MainView::stopBitsSpinHandler));
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
        this->connectButtonListener();
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
}