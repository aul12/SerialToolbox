/**
 * @file ByteRepresentationWidget.hpp
 * @author paul
 * @date 05.01.19
 * @brief Declares a custom GtkBox for representing a item in four different formats
 */

#ifndef SERIALTOOLBOX_BYTEREPRESENTATIONWIDGET_HPP
#define SERIALTOOLBOX_BYTEREPRESENTATIONWIDGET_HPP

#include <string>
#include <memory>

#include <QVBoxLayout>
#include <QLabel>

namespace view {
    class ByteRepresentationWidget : public QVBoxLayout {
    public:
        explicit ByteRepresentationWidget(const std::string &ascii,
                const std::string &dec, const std::string &bin, const std::string &hex);

        void setVisibilityHex(bool visibility);
        void setVisibilityDec(bool visibility);
        void setVisibilityBin(bool visibility);
        void setVisibilityAscii(bool visibility);
    private:
        std::unique_ptr<QLabel> labelHex, labelDec, labelBin, labelAscii;
        bool hexVis, decVis, binVis, asciiVis;
    };
}

#endif //SERIALTOOLBOX_BYTEREPRESENTATIONWIDGET_HPP
