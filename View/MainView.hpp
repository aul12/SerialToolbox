/**
 * @file MainView.hpp
 * @author paul
 * @date 30.12.18
 * @brief MainView @TODO
 */

#ifndef HTERMCLONE_MAINVIEW_HPP
#define HTERMCLONE_MAINVIEW_HPP


#include <gtkmm.h>
#include "../Util/Listener.hpp"

namespace view {
    class MainView {
    public:
        explicit MainView(const std::string &uiFile);

        void setPorts(const std::vector<std::string> &ports, int activeIndex = 0);

        auto getWindow() -> Gtk::Window&;

        const util::Listener<> registerConnectButtonListener;
        const util::Listener<std::string> registerPortComboListener;
        const util::Listener<int> registerBaudSpinListener;
        const util::Listener<int> registerDataBitsSpinListener;
        const util::Listener<int> registerStopBitsSpinListener;

    private:
        Gtk::Window *mainWindow;

        // Top bar
        Gtk::Button *connectButton;
        Gtk::ComboBoxText *portCombo;
        Gtk::SpinButton *baudSpin;
        Gtk::SpinButton *dataBitsSpin;
        Gtk::SpinButton *stopBitsSpin;

        void connectButtonHandler();
        void portComboHandler();
        void baudSpinHandler();
        void dataBitsSpinHandler();
        void stopBitsSpinHandler();
    };
}

#endif
