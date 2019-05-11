#include <utility>

/**
 * @file RepresentationFlowView.cpp
 * @author paul
 * @date 11.05.19
 * @brief RepresentationFlowView @TODO
 */

#include "RepresentationFlowView.hpp"

#include <QHeaderView>
#include <QScrollBar>

RepresentationFlowView::RepresentationFlowView(std::shared_ptr<QTableView> view,
        const std::shared_ptr<QWidget>& mainWindow, int flowWidth)
        : view{std::move(view)}, visAscii{true}, visDec{true},
        visHex{true}, visBin{true}, flowWidth{flowWidth} {
   this->model = std::make_unique<QStandardItemModel>(0, 0, mainWindow.get());
   this->view->setModel(this->model.get());
   this->view->setEditTriggers(QAbstractItemView::NoEditTriggers);
   this->view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void RepresentationFlowView::setVisibility(bool ascii, bool dec, bool hex, bool bin) {
    visAscii = ascii;
    visDec = dec;
    visHex = hex;
    visBin = bin;
    for (auto c=0; c<this->model->rowCount(); c+=5) {
        this->showHide(ascii, c);
        this->showHide(dec, c+1);
        this->showHide(bin, c+2);
        this->showHide(hex, c+3);
    }
}

void RepresentationFlowView::add(const std::string& ascii, const std::string& dec,
        const std::string& hex, const std::string& bin, bool addNewLine) {

    auto *item = new QStandardItem(ascii.c_str());
    item->setBackground(QColor{0x0D, 0xB4, 0xFF});
    item->setForeground(QColor{Qt::white});
    this->model->setItem(position.second*5, position.first, item);

    item = new QStandardItem(dec.c_str());
    item->setBackground(QColor{0xE8, 0x8F, 0x0C});
    item->setForeground(QColor{Qt::white});
    this->model->setItem(position.second*5+1, position.first, item);

    item = new QStandardItem(bin.c_str());
    item->setBackground(QColor{0xFF, 0x00, 0x00});
    item->setForeground(QColor{Qt::white});
    this->model->setItem(position.second*5+2, position.first, item);

    item = new QStandardItem(hex.c_str());
    item->setBackground(QColor{0x60, 0x00, 0xE8});
    item->setForeground(QColor{Qt::white});
    this->model->setItem(position.second*5+3, position.first, item);

    if (this->position.first == 0) {
        this->showHide(visAscii, position.second*5);
        this->showHide(visDec, position.second*5+1);
        this->showHide(visBin, position.second*5+2);
        this->showHide(visHex, position.second*5+3);
    }

    this->view->update();

    auto scrollbar = this->view->verticalScrollBar();
    if (scrollbar->value() == scrollbar->maximum()) {
        this->view->scrollToBottom();
    }

    position.first++;
    if (addNewLine || position.first >= flowWidth) {
        position.first = 0;
        position.second++;
    }
}

void RepresentationFlowView::clear() {
    this->model->clear();
    this->position = std::make_pair(0,0);
}

void RepresentationFlowView::showHide(bool show, int row) {
    if (show) {
        this->view->showRow(row);
    } else {
        this->view->hideRow(row);
    }
}
