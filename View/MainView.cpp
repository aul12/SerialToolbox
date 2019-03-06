/**
 * @file MainView.cpp
 * @author paul
 * @date 30.12.18
 * @brief Wrapper for the MainView (view)
 */

#include "MainView.hpp"

#include <cassert>

#include <QFile>
#include <QUiLoader>
#include <QMessageBox>

#define FIND_WIDGET(x) \
x=std::unique_ptr<decltype(x)::element_type>(mainWindow->findChild<decltype(x)::element_type*>(#x)); \
assert(x != nullptr)

namespace view {
    MainView::MainView(const std::string &uiFile) : receivePosition{0,0}, sendPosition{0,0} {
        QFile file{uiFile.c_str()};
        if(!file.open(QFile::ReadOnly)) {
            throw std::runtime_error("Could not find ui file");
        }
        QUiLoader loader;

        mainWindow = std::unique_ptr<QWidget>(loader.load(&file));
        file.close();
        mainWindow->showMaximized();

        FIND_WIDGET(portCombo);
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
        FIND_WIDGET(sendGrid);
        FIND_WIDGET(receiveGrid);

        mainWindow->connect(portCombo.get(),  QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int){
            portComboHandler();
        });
        mainWindow->connect(baudSpin.get(),  QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int){
            baudSpinHandler();
        });
        mainWindow->connect(dataBitsSpin.get(),  QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int){
            dataBitsSpinHandler();
        });
        mainWindow->connect(stopBitsSpin.get(),  QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int){
            stopBitsSpinHandler();
        });

        mainWindow->connect(checkAscii.get(),  QOverload<int>::of(&QCheckBox::stateChanged), this, [this](int){
            checkAsciiHandler();
        });
        mainWindow->connect(checkBin.get(),  QOverload<int>::of(&QCheckBox::stateChanged), this, [this](int){
            checkBinHandler();
        });
        mainWindow->connect(checkDec.get(),  QOverload<int>::of(&QCheckBox::stateChanged), this, [this](int){
            checkDecHandler();
        });
        mainWindow->connect(checkHex.get(),  QOverload<int>::of(&QCheckBox::stateChanged), this, [this](int){
            checkHexHandler();
        });

        mainWindow->connect(sendButton.get(),  QOverload<bool>::of(&QPushButton::clicked), this, [this](bool){
            sendHandler();
        });

        this->addReceived("A", "123", "7F", "01101100");
        this->addSend("B", "123", "7F", "01101100");

        this->representationIds.insert({"ASCII", 0});
        this->representationIds.insert({"HEX", 1});
        this->representationIds.insert({"DEC", 2});
        this->representationIds.insert({"BIN", 3});

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

    void MainView::portComboHandler() {
        this->portComboListener(this->portCombo->currentText().toLocal8Bit().data());
    }

    void MainView::baudSpinHandler() {
        baudSpinListener(this->baudSpin->value());
    }

    void MainView::dataBitsSpinHandler() {
        dataBitsSpinListener(this->dataBitsSpin->value());
    }

    void MainView::stopBitsSpinHandler() {
        stopBitsSpinListener(this->stopBitsSpin->value());
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

    void MainView::setSerialOptionsVisibility(bool visible) {
        this->baudSpin->setVisible(visible);
        this->stopBitsSpin->setVisible(visible);
        this->dataBitsSpin->setVisible(visible);
        this->parityCombo->setVisible(visible);
        this->stopBitsLabel->setVisible(visible);
        this->dataBitsLabel->setVisible(visible);
    }

    void MainView::addSendImpl(std::string ascii, std::string dec, std::string hex, std::string bin, bool addNewLine) {
        sendWidgets.emplace_back(std::make_unique<ByteRepresentationWidget>(ascii, dec, bin, hex));
        sendWidgets.back()->setVisibilityBin(this->getBinEnabled());
        sendWidgets.back()->setVisibilityDec(this->getDecEnabled());
        sendWidgets.back()->setVisibilityHex(this->getHexEnabled());
        sendWidgets.back()->setVisibilityAscii(this->getAsciiEnabled());
        this->sendGrid->addLayout(sendWidgets.back().get(), sendPosition.second, sendPosition.first);

        sendPosition.first++;
        if (addNewLine) {
            sendPosition.first = 0;
            sendPosition.second++;
        } else if (sendPosition.first >= flowWidth) {
            sendPosition.first -= flowWidth;
            sendPosition.second++;
        }
    }

    void MainView::addReceivedImpl(std::string ascii, std::string dec, std::string hex, std::string bin, bool addNewLine) {
        receiveWidgets.emplace_back(std::make_unique<ByteRepresentationWidget>(ascii, dec, bin, hex));
        receiveWidgets.back()->setVisibilityBin(this->getBinEnabled());
        receiveWidgets.back()->setVisibilityDec(this->getDecEnabled());
        receiveWidgets.back()->setVisibilityHex(this->getHexEnabled());
        receiveWidgets.back()->setVisibilityAscii(this->getAsciiEnabled());
        this->receiveGrid->addLayout(receiveWidgets.back().get(), receivePosition.second, receivePosition.first);

        receivePosition.first++;
        if (addNewLine) {
            receivePosition.first = 0;
            receivePosition.second++;
        } else if (receivePosition.first >= flowWidth) {
            receivePosition.first -= flowWidth;
            receivePosition.second++;
        }
    }

    void MainView::showError(std::string title, std::string message) {
        QMessageBox::critical(mainWindow.get(), title.c_str(), message.c_str(), QMessageBox::Ok);
    }

    void MainView::setVisibility(bool ascii, bool dec, bool hex, bool bin) {
        for (auto &widget : this->sendWidgets) {
            widget->setVisibilityAscii(ascii);
            widget->setVisibilityBin(bin);
            widget->setVisibilityDec(dec);
            widget->setVisibilityHex(hex);
        }

        for (auto &widget: this->receiveWidgets) {
            widget->setVisibilityAscii(ascii);
            widget->setVisibilityBin(bin);
            widget->setVisibilityDec(dec);
            widget->setVisibilityHex(hex);
        }
    }

    void MainView::checkAsciiHandler() {
        this->asciiEnabledListener(this->getAsciiEnabled());
    }

    void MainView::checkHexHandler() {
        this->hexEnabledListener(this->getHexEnabled());
    }

    void MainView::checkDecHandler() {
        this->decEnabledListener(this->getDecEnabled());
    }

    void MainView::checkBinHandler() {
        this->binEnabledListener(this->getBinEnabled());
    }

    void MainView::addReceived(std::string ascii, std::string dec, std::string hex, std::string bin, bool addNewLine) {
        listLock.lock();
        toCall.emplace_back(std::bind(&MainView::addReceivedImpl, this, ascii, dec, hex, bin, addNewLine));
        listLock.unlock();
        QMetaObject::invokeMethod(this, "mainThreadHandler", Qt::QueuedConnection);
    }

    void MainView::addSend(std::string ascii, std::string dec, std::string hex, std::string bin, bool addNewLine) {
        listLock.lock();
        toCall.emplace_back(std::bind(&MainView::addSendImpl, this, ascii, dec, hex, bin, addNewLine));
        listLock.unlock();
        QMetaObject::invokeMethod(this, "mainThreadHandler", Qt::QueuedConnection);
    }

    void MainView::mainThreadHandler() {
        if(this->listLock.try_lock()) {
            for (const auto &f : this->toCall) {
                f();
            }
            this->toCall.clear();
            listLock.unlock();
        }

    }
}