/**
 * @file ByteRepresentationWidget.cpp
 * @author paul
 * @date 05.01.19
 * @brief ByteRepresentationWidget @TODO
 */

#include "ByteRepresentationWidget.hpp"

namespace view {
    ByteRepresentationWidget::ByteRepresentationWidget(const std::string &ascii, const std::string &dec,
                                                       const std::string &bin, const std::string &hex) {
        labelAscii.set_text(ascii);
        labelDec.set_text(dec);
        labelBin.set_text(bin);
        labelHex.set_text(hex);

        labelAscii.override_background_color(Gdk::RGBA("#0DB4FF"));
        labelDec.override_background_color(Gdk::RGBA("#E88F0C"));
        labelBin.override_background_color(Gdk::RGBA("#FF0000"));
        labelHex.override_background_color(Gdk::RGBA("#6000E8"));

        this->box.set_orientation(Gtk::ORIENTATION_VERTICAL);
        this->box.add(labelAscii);
        this->box.add(labelDec);
        this->box.add(labelBin);
        this->box.add(labelHex);

        labelAscii.show();
        labelDec.show();
        labelBin.show();
        labelHex.show();
    }

    void ByteRepresentationWidget::show() {
        this->box.show_all();
    }

    ByteRepresentationWidget::operator Gtk::Box &() {
        return this->box;
    }
}

