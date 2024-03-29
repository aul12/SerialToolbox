/**
 * @file MainView.cpp
 * @author paul
 * @date 30.12.18
 * @brief Wrapper for the MainView (view)
 */

#include "MainView.hpp"

#include <QFile>
#include <QHeaderView>
#include <QMessageBox>
#include <QScrollBar>
#include <QUiLoader>
#include <cassert>

// NOLINTNEXTLINE
#define FIND_WIDGET(widget) widget = this->setWidget<decltype(widget)::element_type>(#widget)

namespace view {
    MainView::MainView(const std::string &uiFile) {
        QFile file{uiFile.c_str()};
        if (!file.open(QFile::ReadOnly)) {
            throw std::runtime_error("Could not find ui file");
        }
        QUiLoader loader;

        mainWindow = std::shared_ptr<QWidget>(loader.load(&file));
        file.close();
        mainWindow->showMaximized();

        FIND_WIDGET(portCombo);
        FIND_WIDGET(refreshButton);
        FIND_WIDGET(connectButton);
        FIND_WIDGET(parityCombo);
        FIND_WIDGET(baudSpin);
        FIND_WIDGET(dataBitsSpin);
        FIND_WIDGET(stopBitsSpin);
        FIND_WIDGET(baudLabel);
        FIND_WIDGET(parityLabel);
        FIND_WIDGET(stopBitsLabel);
        FIND_WIDGET(dataBitsLabel);
        FIND_WIDGET(checkBin);
        FIND_WIDGET(checkDec);
        FIND_WIDGET(checkHex);
        FIND_WIDGET(checkAscii);
        FIND_WIDGET(encodingSendCombo);
        FIND_WIDGET(toSendEntry);
        FIND_WIDGET(repetitionsSpin);
        FIND_WIDGET(periodSpin);
        FIND_WIDGET(sendButton);
        FIND_WIDGET(comboLinebreak);
        FIND_WIDGET(labelRxCount);
        FIND_WIDGET(labelTxCount);
        FIND_WIDGET(buttonResetRx);
        FIND_WIDGET(buttonResetTx);
        FIND_WIDGET(buttonClearReceived);
        FIND_WIDGET(buttonClearSent);
        FIND_WIDGET(receiveView);
        FIND_WIDGET(sendView);

        mainWindow->connect(portCombo.get(), QOverload<int>::of(&QComboBox::currentIndexChanged), this,
                            [this](int index) {
                                auto port = portCombo->itemText(index);
                                portComboListener(port.toLocal8Bit().data());
                            });
        mainWindow->connect(refreshButton.get(), QOverload<bool>::of(&QPushButton::clicked), this,
                            [this](bool /*clicked*/) { refreshListener(); });
        mainWindow->connect(connectButton.get(), QOverload<bool>::of(&QPushButton::clicked), this,
                            [this](bool /*clicked*/) { connectListener(); });
        mainWindow->connect(baudSpin.get(), &QSpinBox::editingFinished, this, [this]() { baudSpinListener(); });
        mainWindow->connect(dataBitsSpin.get(), &QSpinBox::editingFinished, this, [this]() { dataBitsSpinListener(); });
        mainWindow->connect(stopBitsSpin.get(), &QSpinBox::editingFinished, this, [this]() { stopBitsSpinListener(); });
        mainWindow->connect(parityCombo.get(), QOverload<int>::of(&QComboBox::currentIndexChanged), this,
                            [this](int sel) { parityListener(sel); });

        mainWindow->connect(checkAscii.get(), QOverload<int>::of(&QCheckBox::stateChanged), this,
                            [this](int state) { asciiEnabledListener(state == Qt::Checked); });
        mainWindow->connect(checkBin.get(), QOverload<int>::of(&QCheckBox::stateChanged), this,
                            [this](int state) { binEnabledListener(state == Qt::Checked); });
        mainWindow->connect(checkDec.get(), QOverload<int>::of(&QCheckBox::stateChanged), this,
                            [this](int state) { decEnabledListener(state == Qt::Checked); });
        mainWindow->connect(checkHex.get(), QOverload<int>::of(&QCheckBox::stateChanged), this,
                            [this](int state) { hexEnabledListener(state == Qt::Checked); });

        mainWindow->connect(comboLinebreak.get(), QOverload<int>::of(&QComboBox::currentIndexChanged), this,
                            [this](int index) { linebreakListener(index); });

        mainWindow->connect(sendButton.get(), QOverload<bool>::of(&QPushButton::clicked), this,
                            [this](bool /*clicked*/) { sendHandler(); });

        mainWindow->connect(buttonResetRx.get(), QOverload<bool>::of(&QPushButton::clicked), this,
                            [this](bool /*clicked*/) { resetRxListener(); });

        mainWindow->connect(buttonResetTx.get(), QOverload<bool>::of(&QPushButton::clicked), this,
                            [this](bool /*clicked*/) { resetTxListener(); });

        mainWindow->connect(buttonClearSent.get(), QOverload<bool>::of(&QPushButton::clicked), this,
                            [this](bool /*clicked*/) { clearTxListener(); });

        mainWindow->connect(buttonClearReceived.get(), QOverload<bool>::of(&QPushButton::clicked), this,
                            [this](bool /*clicked*/) { clearRxListener(); });

        this->representationIds.insert({"ASCII", 0});
        this->representationIds.insert({"HEX", 1});
        this->representationIds.insert({"DEC", 2});
        this->representationIds.insert({"BIN", 3});

        this->receiveFlowView = std::make_unique<RepresentationFlowView>(receiveView, mainWindow, flowWidth);
        this->sendFlowView = std::make_unique<RepresentationFlowView>(sendView, mainWindow, flowWidth);
    }

    void MainView::setPorts(const std::vector<std::string> &ports, int activeIndex) {
        this->portCombo->clear();
        QStringList stringList;
        for (const auto &port : ports) {
            stringList << port.c_str();
        }
        this->portCombo->addItems(stringList);
        this->portCombo->setCurrentIndex(activeIndex);
    }

    void MainView::sendHandler() {
        std::string repr = this->encodingSendCombo->currentText().toLocal8Bit().data();
        std::string data = this->toSendEntry->text().toLocal8Bit().data();
        int repe = this->repetitionsSpin->value();
        int inte = this->periodSpin->value();
        auto reprId = representationIds.find(repr);
        if (reprId == representationIds.end()) {
            throw std::runtime_error{"Invalid key!"};
        }

        sendClickListener(reprId->second, data, repe, inte);
    }

    auto MainView::getPort() const -> std::string {
        return this->portCombo->currentText().toLocal8Bit().data();
    }

    auto MainView::getBaud() const -> int {
        return this->baudSpin->value();
    }

    auto MainView::getDataBits() const -> int {
        return this->dataBitsSpin->value();
    }

    auto MainView::getStopBits() const -> int {
        return this->stopBitsSpin->value();
    }

    auto MainView::getLinebreak() const -> int {
        return this->comboLinebreak->currentIndex();
    }

    auto MainView::getAsciiEnabled() const -> bool {
        return this->checkAscii->isChecked();
    }

    auto MainView::getHexEnabled() const -> bool {
        return this->checkHex->isChecked();
    }

    auto MainView::getDecEnabled() const -> bool {
        return this->checkDec->isChecked();
    }

    auto MainView::getBinEnabled() const -> bool {
        return this->checkBin->isChecked();
    }

    void MainView::setRxCount(std::size_t count) {
        std::unique_lock guard{listLock};
        toCall.emplace_back([this, count] { setRxCountImpl(count); });
        guard.unlock();
        QMetaObject::invokeMethod(this, "mainThreadHandler", Qt::QueuedConnection);
    }

    void MainView::setTxCount(std::size_t count) {
        std::unique_lock guard{listLock};
        toCall.emplace_back([this, count] { setTxCountImpl(count); });
        guard.unlock();
        QMetaObject::invokeMethod(this, "mainThreadHandler", Qt::QueuedConnection);
    }

    void MainView::setRxCountImpl(std::size_t count) {
        this->labelRxCount->setText(QStringLiteral("RX: %1").arg(count));
    }

    void MainView::setTxCountImpl(std::size_t count) {
        this->labelTxCount->setText(QStringLiteral("TX: %1").arg(count));
    }

    void MainView::setConnectButtonText(const std::string &text) {
        this->connectButton->setText(text.c_str());
    }

    void MainView::addSendImpl(const std::string &ascii, const std::string &dec, const std::string &hex,
                               const std::string &bin, bool addNewLine) {
        this->sendFlowView->add(ascii, dec, hex, bin, addNewLine);
    }

    void MainView::addReceivedImpl(const std::string &ascii, const std::string &dec, const std::string &hex,
                                   const std::string &bin, bool addNewLine) {
        this->receiveFlowView->add(ascii, dec, hex, bin, addNewLine);
    }

    void MainView::clearReceived() {
        this->receiveFlowView->clear();
    }

    void MainView::clearSent() {
        this->sendFlowView->clear();
    }

    void MainView::showErrorImpl(const std::string &title, const std::string &message) {
        QMessageBox::critical(mainWindow.get(), title.c_str(), message.c_str(), QMessageBox::Ok);
    }

    void MainView::setVisibility(bool ascii, bool dec, bool hex, bool bin) {
        this->receiveFlowView->setVisibility(ascii, dec, hex, bin);
        this->sendFlowView->setVisibility(ascii, dec, hex, bin);
    }

    void MainView::addReceived(const std::string &ascii, const std::string &dec, const std::string &hex,
                               const std::string &bin, bool addNewLine) {
        std::unique_lock guard{listLock};
        toCall.emplace_back(
                [this, ascii, dec, hex, bin, addNewLine] { addReceivedImpl(ascii, dec, hex, bin, addNewLine); });
        guard.unlock();
        QMetaObject::invokeMethod(this, "mainThreadHandler", Qt::QueuedConnection);
    }

    void MainView::addSend(const std::string &ascii, const std::string &dec, const std::string &hex,
                           const std::string &bin, bool addNewLine) {
        std::unique_lock guard{listLock};
        toCall.emplace_back(
                [this, ascii, dec, hex, bin, addNewLine] { addSendImpl(ascii, dec, hex, bin, addNewLine); });
        guard.unlock();
        QMetaObject::invokeMethod(this, "mainThreadHandler", Qt::QueuedConnection);
    }

    void MainView::showError(const std::string &title, const std::string &message) {
        std::unique_lock guard{listLock};
        toCall.emplace_back([this, title, message] { showErrorImpl(title, message); });
        guard.unlock();
        QMetaObject::invokeMethod(this, "mainThreadHandler", Qt::QueuedConnection);
    }

    void MainView::mainThreadHandler() {
        if (this->listLock.try_lock()) {
            for (const auto &f : this->toCall) {
                f();
            }
            this->toCall.clear();
            listLock.unlock();
        }
    }

    template<typename QThing>
    auto MainView::setWidget(const char *name) -> std::unique_ptr<QThing> {
        auto widget = std::unique_ptr<QThing>(mainWindow->findChild<QThing *>(name));
        assert(widget != nullptr);
        return widget;
    }
} // namespace view
