/**
 * @file LineFeedStateMachine.hpp
 * @author paul
 * @date 07.03.19
 * @brief LineFeedStateMachine @TODO
 */

#ifndef SERIALTOOLBOX_LINEFEEDSTATEMACHINE_HPP
#define SERIALTOOLBOX_LINEFEEDSTATEMACHINE_HPP

#include <string>

namespace controller {
    enum class LinebreakType { NONE = 0, LF = 1, CRLF = 2, CR = 3, LFCR = 4 };

    enum class LinebreakState { NONE, CR, LF };

    class LineBreakStateMachine {
      public:
        explicit LineBreakStateMachine(LinebreakType linebreak);
        auto addAscii(const std::string &ascii) -> bool;
        void setLinebreak(LinebreakType linebreak);
        void reset();

      private:
        LinebreakType type;
        LinebreakState state;
    };
} // namespace controller


#endif
