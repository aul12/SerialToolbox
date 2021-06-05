#include <QDebug>
#include <QWidget>
#include <QtCore/QFile>
#include <QtUiTools/QUiLoader>
#include <QtWidgets/QApplication>
#include <iostream>
#include <memory>

#include "Controller/UiController.hpp"
#include "Util/Serial/InterfacePosix.hpp"
#include "View/MainView.hpp"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    std::string uiFile = UI_FILE_LOCATION;
    if (argc == 2) {
        uiFile = argv[1];
    }
    auto mainView = std::make_shared<view::MainView>(uiFile);
    controller::UiController uiController{mainView};

    QApplication::exec();

    return 0;
}
