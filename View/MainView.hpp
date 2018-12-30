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
        explicit MainView(const std::string &uiFile);

        void setPorts(const std::vector<std::string> &ports, int activeIndex = 0);

        auto getWindow() -> Gtk::Window&;

        void registerConnectButtonListener(std::function<void()> listener);
    private:
        Gtk::Window *mainWindow;

        // Top bar
        Gtk::ComboBoxText *portCombo;
        Gtk::SpinButton *baudSpin;
        Gtk::Button *connectButton;
        Gtk::SpinButton *dataBitsSpin;
        Gtk::SpinButton *stopBitsSpin;

        void connectButtonHandler();
        std::optional<std::function<void()>> connectButtonListener;
    };
}

#endif //HTERMCLONE_MAINVIEW_HPP
