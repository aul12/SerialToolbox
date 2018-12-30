/**
 * @file MainView.cpp
 * @author paul
 * @date 30.12.18
 * @brief MainView @TODO
 */

#include <gtkmm/builder.h>
#include "MainView.hpp"

namespace view {
    MainView::MainView(const std::string &uiFile, Glib::RefPtr<Gtk::Application> app) {
        Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file(uiFile);

        builder->get_widget("MainWindow", window);
        app->run(*window);
    }
}