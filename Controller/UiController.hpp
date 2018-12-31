/**
 * @file UiController.hpp
 * @author paul
 * @date 31.12.18
 * @brief UiController @TODO
 */

#ifndef HTERMCLONE_UICONTROLLER_HPP
#define HTERMCLONE_UICONTROLLER_HPP

#include "../View/MainView.hpp"

namespace controller {
    class UiController {
    public:
        UiController(std::shared_ptr<view::MainView> mainView);

    private:
        std::shared_ptr<view::MainView> mainView;
    };
}

#endif
