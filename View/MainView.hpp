/**
 * @file MainView.hpp
 * @author paul
 * @date 30.12.18
 * @brief MainView @TODO
 */

#ifndef HTERMCLONE_MAINVIEW_HPP
#define HTERMCLONE_MAINVIEW_HPP


#include <gtkmm.h>

namespace view {
    class MainView {
    public:
        MainView(const std::string &uiFile, Glib::RefPtr<Gtk::Application> app);
    private:
        Gtk::Window *window;
    };
}

#endif //HTERMCLONE_MAINVIEW_HPP
