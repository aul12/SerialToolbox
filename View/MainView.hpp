/**
 * @file MainView.hpp
 * @author paul
 * @date 30.12.18
 * @brief Declaration of the MainView class
 */

#ifndef SERIALTOOLBOX_MAINVIEW_HPP
#define SERIALTOOLBOX_MAINVIEW_HPP

#include <gtkmm.h>
#include "../Util/Listener.hpp"
#include "ByteRepresentationWidget.hpp"

namespace view {
    class MainView {
    public:
        explicit MainView(const std::string &uiFile);

        void setPorts(const std::vector<std::string> &ports, int activeIndex = 0);

        auto getWindow() -> Gtk::Window&;

        // Top Bar
        const util::Listener<std::string> portComboListener;
        const util::Listener<int> baudSpinListener;
        const util::Listener<int> dataBitsSpinListener;
        const util::Listener<int> stopBitsSpinListener;
        const util::Listener<int, const std::string&, int, int> sendClickListener;

        auto getPort() const -> std::string;
        auto getBaud() const -> int;
        auto getDataBits() const -> int;
        auto getStopBits() const -> int;

        void setSerialOptionsVisibility(bool visible);

        // Sidebar
        const util::Listener<bool> asciiEnabledListener;
        const util::Listener<bool> hexEnabledListener;
        const util::Listener<bool> decEnabledListener;
        const util::Listener<bool> binEnabledListener;

        auto getAsciiEnabled() const -> bool;
        auto getHexEnabled() const -> bool;
        auto getDecEnabled() const -> bool;
        auto getBinEnabled() const -> bool;

        // Center
        void addReceived(std::string ascii, std::string dec, std::string hex, std::string bin);
        void addSend(std::string ascii, std::string dec, std::string hex, std::string bin);
        void setVisibility(bool ascii, bool dec, bool hex, bool bin);

        // Util
        void showError(std::string title, std::string message);
    private:
        Gtk::Window *mainWindow;

        // Top bar
        Gtk::ComboBoxText *portCombo;
        Gtk::ComboBoxText *parityCombo;
        Gtk::SpinButton *baudSpin;
        Gtk::SpinButton *dataBitsSpin;
        Gtk::SpinButton *stopBitsSpin;
        Gtk::Label *stopBitsLabel;
        Gtk::Label *dataBitsLabel;

        void portComboHandler();
        void baudSpinHandler();
        void dataBitsSpinHandler();
        void stopBitsSpinHandler();
        void sendHandler();

        // Sidebar
        Gtk::CheckButton *checkAscii;
        Gtk::CheckButton *checkHex;
        Gtk::CheckButton *checkDec;
        Gtk::CheckButton *checkBin;

        void checkAsciiHandler();
        void checkHexHandler();
        void checkDecHandler();
        void checkBinHandler();

        // Receive
        Gtk::FlowBox *receiveFlow;

        // Send
        Gtk::FlowBox *sendFlow;
        Gtk::ComboBoxText *encodingSendCombo;
        Gtk::Entry *toSendEntry;
        Gtk::SpinButton *repetitionsSpin;
        Gtk::SpinButton *periodSpin;
        Gtk::Button *sendButton;

        // All widgets need to exist while they are visible
        std::list<ByteRepresentationWidget> sendWidgets, receiveWidgets;

        std::map<std::string, int> representationIds;

    };
}

#endif
