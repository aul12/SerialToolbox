/**
 * @file RepresentationFlowView.hpp
 * @author paul
 * @date 11.05.19
 * @brief RepresentationFlowView @TODO
 */

#ifndef SERIALTOOLBOX_REPRESENTATIONFLOWVIEW_HPP
#define SERIALTOOLBOX_REPRESENTATIONFLOWVIEW_HPP

#include <memory>
#include <QTableView>
#include <QStandardItemModel>

class RepresentationFlowView {
public:
    RepresentationFlowView(std::shared_ptr<QTableView> view, const std::shared_ptr<QWidget>& mainWindow,
            int flowWidth);
    void setVisibility(bool ascii, bool dec, bool hex, bool bin);
    void add(const std::string& ascii, const std::string& dec, const std::string& hex, const std::string& bin, bool addNewLine = false);
    void clear();
private:
    void showHide(bool show, int row);
    std::shared_ptr<QTableView> view;
    std::unique_ptr<QStandardItemModel> model;
    std::pair<int,int> position;
    bool visAscii, visDec, visHex, visBin;
    const int flowWidth;
};


#endif //SERIALTOOLBOX_REPRESENTATIONFLOWVIEW_HPP
