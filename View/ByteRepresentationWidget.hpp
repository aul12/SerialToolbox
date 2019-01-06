/**
 * @file ByteRepresentationWidget.hpp
 * @author paul
 * @date 05.01.19
 * @brief ByteRepresentationWidget @TODO
 */

#ifndef SERIALTOOLBOX_BYTEREPRESENTATIONWIDGET_HPP
#define SERIALTOOLBOX_BYTEREPRESENTATIONWIDGET_HPP

#include <string>
#include <gtkmm.h>

namespace view {
    class ByteRepresentationWidget {
    public:
        explicit ByteRepresentationWidget(const std::string &ascii,
                const std::string &dec, const std::string &bin, const std::string &hex);
        operator Gtk::Box&();
        void show();
    private:
        Gtk::Box box;
        Gtk::Label labelHex, labelDec, labelBin, labelAscii;
    };
}

#endif //SERIALTOOLBOX_BYTEREPRESENTATIONWIDGET_HPP
