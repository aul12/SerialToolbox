#include <iostream>
#include <memory>
#include <gtkmm.h>
#include "Util/Serial/InterfacePosix.hpp"
#include "View/MainView.hpp"
#include "Controller/UiController.hpp"


int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "me.aul12.term");

    auto mainView = std::make_shared<view::MainView>("Res/ui.glade");
    controller::UiController uiController{mainView};
    app->run(mainView->getWindow());

    return 0;
}