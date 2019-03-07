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
        if (ascii != "CR" && ascii != "LF") {
            this->state = LinebreakState::NONE;
            return false;
        }

        switch (this->type) {
            case LinebreakType::NONE:
                return false;
            case LinebreakType::LF:
                return ascii == "LF";
            case LinebreakType::CR:
                return ascii == "CR";
            case LinebreakType::CRLF:
            case LinebreakType::LFCR:
                if (this->state == LinebreakState::CR && this->type == LinebreakType::CRLF &&
                            ascii == "LF") {
                    this->state = LinebreakState::NONE;
                    return true;
                } else if (this->state == LinebreakState::LF && this->type == LinebreakType::LFCR &&
                            ascii == "CR") {
                    this->state = LinebreakState::NONE;
                    return true;
                } else {
                    if (ascii == "LF") {
                        this->state = LinebreakState::LF;
                    } else {
                        this->state = LinebreakState::CR;
                    }
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
