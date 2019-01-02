/**
 * @file UiController.hpp
 * @author paul
 * @date 31.12.18
 * @brief UiController @TODO
 */

#ifndef HTERMCLONE_UICONTROLLER_HPP
#define HTERMCLONE_UICONTROLLER_HPP

#include "SerialProxy.hpp"
#include "../View/MainView.hpp"

namespace controller {
    class UiController {
    public:
        explicit UiController(const std::shared_ptr<view::MainView> &mainView);

    private:
        std::shared_ptr<view::MainView> mainView;
        std::shared_ptr<util::serial::Interface> interface;
        std::shared_ptr<controller::SerialProxy> serialProxy;
    };
}

#endif
