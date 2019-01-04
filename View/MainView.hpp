/**
 * @file MainView.hpp
 * @author paul
 * @date 30.12.18
 * @brief MainView @TODO
 */

#ifndef SERIALTOOLBOX_MAINVIEW_HPP
#define SERIALTOOLBOX_MAINVIEW_HPP


#include <gtkmm.h>
#include "../Util/Listener.hpp"

namespace view {
    class MainView {
    public:
        explicit MainView(const std::string &uiFile);

        void setPorts(const std::vector<std::string> &ports, int activeIndex = 0);

        auto getWindow() -> Gtk::Window&;

        const util::Listener<> connectButtonListener;
        const util::Listener<std::string> portComboListener;
        const util::Listener<int> baudSpinListener;
        const util::Listener<int> dataBitsSpinListener;
        const util::Listener<int> stopBitsSpinListener;

        auto getPort() const -> std::string;
        auto getBaud() const -> int;
        auto getDataBits() const -> int;
        auto getStopBits() const -> int;
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
