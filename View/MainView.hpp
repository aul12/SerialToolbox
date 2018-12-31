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
        void registerPortComboListener(std::function<void(std::string)> listener);
        void registerBaudSpinListener(std::function<void(int)> listener);
        void registerDataBitsSpinListener(std::function<void(int)> listener);
        void registerStopBitsSpinListener(std::function<void(int)> listener);
    private:
        Gtk::Window *mainWindow;

        // Top bar
        Gtk::Button *connectButton;
        Gtk::ComboBoxText *portCombo;
        Gtk::SpinButton *baudSpin;
        Gtk::SpinButton *dataBitsSpin;
        Gtk::SpinButton *stopBitsSpin;

        void connectButtonHandler();
        std::optional<std::function<void()>> connectButtonListener;
        void portComboHandler();
        std::optional<std::function<void(std::string)>> portComboListener;
        void baudSpinHandler();
        std::optional<std::function<void(int)>> baudSpinListener;
        void dataBitsSpinHandler();
        std::optional<std::function<void(int)>> dataBitsSpinListener;
        void stopBitsSpinHandler();
        std::optional<std::function<void(int)>> stopBitsSpinListener;
    };
}

#endif //HTERMCLONE_MAINVIEW_HPP
