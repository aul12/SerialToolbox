#include <gtkmm.h>
#include "Util/Serial/InterfacePosix.hpp"
#include "View/MainView.hpp"

using namespace Gtk;

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "me.aul12.term");
    view::MainView mainView{"Res/ui.glade", app};

    return 0;
}