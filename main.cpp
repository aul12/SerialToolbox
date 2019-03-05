#include <iostream>
#include <memory>
#include "Util/Serial/InterfacePosix.hpp"
#include "View/MainView.hpp"
#include "Controller/UiController.hpp"
#include <QDebug>
#include <QtUiTools/QUiLoader>
#include <QtCore/QFile>
#include <QtWidgets/QApplication>
#include <QWidget>


int main(int argc, char *argv[]) {
    QApplication app (argc, argv);

    auto mainView = std::make_shared<view::MainView>("Res/main.ui");
    controller::UiController uiController{mainView};

    QApplication::exec();

    return 0;
}