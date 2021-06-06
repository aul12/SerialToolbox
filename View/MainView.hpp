/**
 * @file MainView.hpp
 * @author paul
 * @date 30.12.18
 * @brief Declaration of the MainView class
 */

#ifndef SERIALTOOLBOX_MAINVIEW_HPP
#define SERIALTOOLBOX_MAINVIEW_HPP

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>
#include <deque>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <mutex>

#include "../Util/Listener.hpp"
#include "RepresentationFlowView.hpp"

namespace view {
    class MainView : private QObject {
        Q_OBJECT
      public:
        explicit MainView(const std::string &uiFile);

        void setPorts(const std::vector<std::string> &ports, int activeIndex = 0);

        // Top Bar
        const util::Listener<std::string> portComboListener;
        const util::Listener<> refreshListener;
        const util::Listener<> connectListener;
        const util::Listener<> baudSpinListener;
        const util::Listener<> dataBitsSpinListener;
        const util::Listener<> stopBitsSpinListener;
        const util::Listener<int> parityListener;

        auto getPort() const -> std::string;
        auto getBaud() const -> int;
        auto getDataBits() const -> int;
        auto getStopBits() const -> int;

        void setConnectButtonText(const std::string &text);

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
        void setRxCount(std::size_t count);
        void setTxCount(std::size_t count);

        // Center
        void setVisibility(bool ascii, bool dec, bool hex, bool bin);
        void addReceived(const std::string &ascii, const std::string &dec, const std::string &hex,
                         const std::string &bin, bool addNewLine = false);
        void addSend(const std::string &ascii, const std::string &dec, const std::string &hex, const std::string &bin,
                     bool addNewLine = false);
        void clearReceived();
        void clearSent();

        const util::Listener<int, const std::string &, int, int> sendClickListener;

        // Util
        void showError(const std::string &title, const std::string &message);

      private:
        void addReceivedImpl(const std::string &ascii, const std::string &dec, const std::string &hex,
                             const std::string &bin, bool addNewLine);
        void addSendImpl(const std::string &ascii, const std::string &dec, const std::string &hex,
                         const std::string &bin, bool addNewLine);
        void showErrorImpl(const std::string &title, const std::string &message);

        std::shared_ptr<QWidget> mainWindow;

        // Top bar
        std::unique_ptr<QComboBox> portCombo;
        std::unique_ptr<QPushButton> refreshButton;
        std::unique_ptr<QPushButton> connectButton;
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

        void setRxCountImpl(std::size_t count);
        void setTxCountImpl(std::size_t count);

        // Receive
        std::shared_ptr<QTableView> receiveView;
        std::unique_ptr<RepresentationFlowView> receiveFlowView;

        // Send
        std::shared_ptr<QTableView> sendView;
        std::unique_ptr<RepresentationFlowView> sendFlowView;
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
} // namespace view

#endif
