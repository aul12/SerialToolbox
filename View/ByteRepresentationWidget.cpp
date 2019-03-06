/**
 * @file ByteRepresentationWidget.cpp
 * @author paul
 * @date 05.01.19
 * @brief Implementation of a custom widget for representing different representations
 */

#include "ByteRepresentationWidget.hpp"

namespace view {
    ByteRepresentationWidget::ByteRepresentationWidget(const std::string &ascii, const std::string &dec,
                                                       const std::string &bin, const std::string &hex) :
                                                       QVBoxLayout{},
                                                       hexVis{true}, decVis{true},
                                                       binVis{true}, asciiVis{true} {
        this->labelAscii = std::make_unique<QLabel>(ascii.c_str());
        this->labelDec = std::make_unique<QLabel>(dec.c_str());
        this->labelBin = std::make_unique<QLabel>(bin.c_str());
        this->labelHex = std::make_unique<QLabel>(hex.c_str());

        this->labelAscii->setStyleSheet("QLabel { background-color : #0DB4FF; color : white; }");
        this->labelDec->setStyleSheet("QLabel { background-color : #E88F0C; color : white; }");
        this->labelBin->setStyleSheet("QLabel { background-color : #FF0000; color : white; }");
        this->labelHex->setStyleSheet("QLabel { background-color : #6000E8; color : white; }");

        this->addWidget(this->labelAscii.get());
        this->addWidget(this->labelDec.get());
        this->addWidget(this->labelBin.get());
        this->addWidget(this->labelHex.get());
    }

    void ByteRepresentationWidget::setVisibilityHex(bool visibility) {
        this->labelHex->setVisible(visibility);
        this->hexVis = visibility;
    }

    void ByteRepresentationWidget::setVisibilityDec(bool visibility) {
        this->labelDec->setVisible(visibility);
        this->decVis = visibility;
    }

    void ByteRepresentationWidget::setVisibilityBin(bool visibility) {
        this->labelBin->setVisible(visibility);
        this->binVis = visibility;
    }

    void ByteRepresentationWidget::setVisibilityAscii(bool visibility) {
        this->labelAscii->setVisible(visibility);
        this->asciiVis = visibility;
    }
}

