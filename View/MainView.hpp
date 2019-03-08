/**
 * @file MainView.hpp
 * @author paul
 * @date 30.12.18
 * @brief Declaration of the MainView class
 */

#ifndef SERIALTOOLBOX_MAINVIEW_HPP
#define SERIALTOOLBOX_MAINVIEW_HPP

#include <map>
#include <memory>
#include <deque>
#include <mutex>
#include <functional>
#include <list>

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QTimer>

#include "../Util/Listener.hpp"
#include "ByteRepresentationWidget.hpp"

namespace view {
    class MainView : private QObject {
        Q_OBJECT
    public:
        explicit MainView(const std::string &uiFile);

        void setPorts(const std::vector<std::string> &ports, int activeIndex = 0);

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
        const util::Listener<int> linebreakListener;
        const util::Listener<> resetRxListener, resetTxListener;
        const util::Listener<> clearRxListener, clearTxListener;

        auto getAsciiEnabled() const -> bool;
        auto getHexEnabled() const -> bool;
        auto getDecEnabled() const -> bool;
        auto getBinEnabled() const -> bool;
        auto getLinebreak() const -> int;
        void setRxCount(int count);
        void setTxCount(int count);

        // Center
        void setVisibility(bool ascii, bool dec, bool hex, bool bin);
        void addReceived(std::string ascii, std::string dec, std::string hex, std::string bin, bool addNewLine = false);
        void addSend(std::string ascii, std::string dec, std::string hex, std::string bin, bool addNewLine = false);
        void clearReceived();
        void clearSent();

        // Util
        void showError(std::string title, std::string message);

    private:
        void addReceivedImpl(std::string ascii, std::string dec, std::string hex, std::string bin, bool addNewLine);
        void addSendImpl(std::string ascii, std::string dec, std::string hex, std::string bin, bool addNewLine);
        void showErrorImpl(std::string title, std::string message);

        std::shared_ptr<QWidget> mainWindow;

        // Top bar
        std::unique_ptr<QComboBox> portCombo;
        std::unique_ptr<QComboBox> parityCombo;
        std::unique_ptr<QSpinBox> baudSpin;
        std::unique_ptr<QSpinBox> dataBitsSpin;
        std::unique_ptr<QSpinBox> stopBitsSpin;
        std::unique_ptr<QLabel> baudLabel, parityLabel, stopBitsLabel, dataBitsLabel;

        // Sidebar
        std::unique_ptr<QCheckBox> checkAscii, checkHex, checkDec, checkBin;
        std::unique_ptr<QComboBox> comboLinebreak;
        std::unique_ptr<QLabel> labelRxCount, labelTxCount;
        std::unique_ptr<QPushButton> buttonResetRx, buttonResetTx;
        std::unique_ptr<QPushButton> buttonClearReceived, buttonClearSent;

        void setRxCountImpl(int count);
        void setTxCountImpl(int count);

        // Receive
        std::unique_ptr<QGridLayout> receiveGrid;
        std::deque<std::unique_ptr<ByteRepresentationWidget>> receiveWidgets;
        std::pair<int,int> receivePosition;

        // Send
        std::unique_ptr<QGridLayout> sendGrid;
        std::deque<std::unique_ptr<ByteRepresentationWidget>> sendWidgets;
        std::pair<int,int> sendPosition;
        std::unique_ptr<QComboBox> encodingSendCombo;
        std::unique_ptr<QLineEdit> toSendEntry;
        std::unique_ptr<QSpinBox> repetitionsSpin, periodSpin;
        std::unique_ptr<QPushButton> sendButton;

        void sendHandler();

        std::map<std::string, int> representationIds;

        static constexpr int flowWidth = 16;

        // UI Updates in the Main-Thread
        std::mutex listLock;
        std::list<std::function<void()>> toCall;
    public slots:
        void mainThreadHandler();
    };
}

#endif
