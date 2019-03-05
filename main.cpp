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
    QUiLoader loader;

    QFile file("Res/main.ui");
    file.open(QFile::ReadOnly);

    std::unique_ptr<QWidget> formWidget{loader.load(&file)};
    file.close();

    formWidget->showMaximized();

    QApplication::exec();
    //auto mainView = std::make_shared<view::MainView>("Res/ui.glade");
    //controller::UiController uiController{mainView};

    return 0;
}