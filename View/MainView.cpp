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
        builder->get_widget(WIDGET_MAP(portCombo));
        builder->get_widget(WIDGET_MAP(baudSpin));
        builder->get_widget(WIDGET_MAP(connectButton));
        builder->get_widget(WIDGET_MAP(dataBitsSpin));
        builder->get_widget(WIDGET_MAP(stopBitsSpin));

        this->connectButton->signal_clicked().connect(sigc::mem_fun(this, &MainView::connectButtonHandler));
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

    void MainView::registerConnectButtonListener(std::function<void()> listener) {
        if (this->connectButtonListener.has_value()) {
            throw std::logic_error("Only on listener possible");
        }

        this->connectButtonListener = listener;
    }
}