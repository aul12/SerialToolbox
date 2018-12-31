#include <iostream>
#include <memory>
#include <gtkmm.h>
#include "Util/Serial/InterfacePosix.hpp"
#include "View/MainView.hpp"


int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "me.aul12.term");

    auto ports = util::serial::InterfacePosix::getAvailablePorts();

    auto mainView = std::make_shared<view::MainView>("Res/ui.glade");
    mainView->setPorts(util::serial::InterfacePosix::getAvailablePorts());
    std::function<void()> test = [](){
        std::cout << "Btn" << std::endl;
    };
    mainView->connectButtonListener(test);
    mainView->connectButtonListener(test);
    app->run(mainView->getWindow());

    return 0;
}