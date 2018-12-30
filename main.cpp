#include <iostream>
#include <gtkmm.h>
#include "Util/Serial/InterfacePosix.hpp"
#include "View/MainView.hpp"

using namespace Gtk;

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "me.aul12.term");

    auto ports = util::serial::InterfacePosix::getAvailablePorts();

    view::MainView mainView{"Res/ui.glade"};
    mainView.setPorts(ports);
    mainView.registerConnectButtonListener([](){
       std::cout << "Clicked" << std::endl;
    });
    app->run(mainView.getWindow());

    return 0;
}