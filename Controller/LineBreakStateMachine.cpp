/**
 * @file LineFeedStateMachine.cpp
 * @author paul
 * @date 07.03.19
 * @brief LineFeedStateMachine @TODO
 */

#include "LineBreakStateMachine.hpp"

namespace controller {
    LineBreakStateMachine::LineBreakStateMachine(LinebreakType linebreak) :
        type{linebreak}, state{LinebreakState::NONE} {}

    auto LineBreakStateMachine::addAscii(std::string ascii) -> bool {
        if (ascii != "\r" && ascii != "\n") {
            this->state = LinebreakState::NONE;
            return false;
        }

        switch (this->type) {
            case LinebreakType::NONE:
                return false;
            case LinebreakType::LF:
                return ascii == "\n";
            case LinebreakType::CR:
                return ascii == "\r";
            case LinebreakType::CRLF:
            case LinebreakType::LFCR:
                if (this->state == LinebreakState::NONE) {
                    if (ascii == "\n") {
                        this->state = LinebreakState::LF;
                    } else {
                        this->state = LinebreakState::CR;
                    }
                } else if (this->state == LinebreakState::CR && this->type == LinebreakType::CRLF &&
                            ascii == "\n") {
                    this->state = LinebreakState::NONE;
                    return true;
                } else if (this->state == LinebreakState::LF && this->type == LinebreakType::LFCR &&
                            ascii == "\r") {
                    this->state = LinebreakState::NONE;
                    return true;
                }
                break;
        }
        return false;
    }

    void LineBreakStateMachine::setLinebreak(LinebreakType linebreak) {
        this->type = linebreak;
    }

    void LineBreakStateMachine::reset() {
        this->state = LinebreakState::NONE;
    }
}
