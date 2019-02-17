/**
 * @file ByteRepresentationWidget.cpp
 * @author paul
 * @date 05.01.19
 * @brief ByteRepresentationWidget @TODO
 */

#include "ByteRepresentationWidget.hpp"

namespace view {
    ByteRepresentationWidget::ByteRepresentationWidget(const std::string &ascii, const std::string &dec,
                                                       const std::string &bin, const std::string &hex) :
                                                       hexVis{true}, decVis{true},
                                                       binVis{true}, asciiVis{true} {
        this->labelAscii.set_text(ascii);
        this->labelDec.set_text(dec);
        this->labelBin.set_text(bin);
        this->labelHex.set_text(hex);

        this->labelAscii.override_background_color(Gdk::RGBA("#0DB4FF"));
        this->labelDec.override_background_color(Gdk::RGBA("#E88F0C"));
        this->labelBin.override_background_color(Gdk::RGBA("#FF0000"));
        this->labelHex.override_background_color(Gdk::RGBA("#6000E8"));

        this->box.set_orientation(Gtk::ORIENTATION_VERTICAL);
        this->box.add(labelAscii);
        this->box.add(labelDec);
        this->box.add(labelBin);
        this->box.add(labelHex);

        this->show();
    }

    void ByteRepresentationWidget::show() {
        this->box.show();
        this->setVisibilityAscii(this->asciiVis);
        this->setVisibilityBin(this->binVis);
        this->setVisibilityDec(this->decVis);
        this->setVisibilityHex(this->hexVis);
    }

    void ByteRepresentationWidget::setVisibilityHex(bool visibility) {
        this->labelHex.set_visible(visibility);
        this->hexVis = visibility;
    }

    void ByteRepresentationWidget::setVisibilityDec(bool visibility) {
        this->labelDec.set_visible(visibility);
        this->decVis = visibility;
    }

    void ByteRepresentationWidget::setVisibilityBin(bool visibility) {
        this->labelBin.set_visible(visibility);
        this->binVis = visibility;
    }

    void ByteRepresentationWidget::setVisibilityAscii(bool visibility) {
        this->labelAscii.set_visible(visibility);
        this->asciiVis = visibility;
    }

    ByteRepresentationWidget::operator Gtk::Box &() {
        return this->box;
    }
}

