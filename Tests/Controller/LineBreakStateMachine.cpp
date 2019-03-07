#include <gtest/gtest.h>

#include "Controller/LineBreakStateMachine.hpp"

TEST(LineBreakStateMachine, NoLineBreakNone) {
    controller::LineBreakStateMachine lineBreakStateMachine{controller::LinebreakType::NONE};
    EXPECT_FALSE(lineBreakStateMachine.addAscii("a"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("b"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("c"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("d"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("n"));
}

TEST(LineBreakStateMachine, NoLineBreakLf) {
    controller::LineBreakStateMachine lineBreakStateMachine{controller::LinebreakType::LF};
    EXPECT_FALSE(lineBreakStateMachine.addAscii("a"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("b"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("c"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("d"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("n"));
}

TEST(LineBreakStateMachine, NoLineBreakLfCr) {
    controller::LineBreakStateMachine lineBreakStateMachine{controller::LinebreakType::LFCR};
    EXPECT_FALSE(lineBreakStateMachine.addAscii("a"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("b"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("c"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("d"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("n"));
}

TEST(LineBreakStateMachine, NoLineBreakCrLf) {
    controller::LineBreakStateMachine lineBreakStateMachine{controller::LinebreakType::CRLF};
    EXPECT_FALSE(lineBreakStateMachine.addAscii("a"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("b"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("c"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("d"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("n"));
}

TEST(LineBreakStateMachine, NoLineBreakCr) {
    controller::LineBreakStateMachine lineBreakStateMachine{controller::LinebreakType::CR};
    EXPECT_FALSE(lineBreakStateMachine.addAscii("a"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("b"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("c"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("d"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("n"));
}

TEST(LineBreakStateMachine, LineBreakCr) {
    controller::LineBreakStateMachine lineBreakStateMachine{controller::LinebreakType::CR};
    EXPECT_FALSE(lineBreakStateMachine.addAscii("a"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("b"));
    EXPECT_TRUE(lineBreakStateMachine.addAscii("CR"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("d"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("LF"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("e"));
    EXPECT_TRUE(lineBreakStateMachine.addAscii("CR"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("LF"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("f"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("LF"));
    EXPECT_TRUE(lineBreakStateMachine.addAscii("CR"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("g"));
}
TEST(LineBreakStateMachine, LineBreakLf) {
    controller::LineBreakStateMachine lineBreakStateMachine{controller::LinebreakType::LF};
    EXPECT_FALSE(lineBreakStateMachine.addAscii("a"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("b"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("CR"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("d"));
    EXPECT_TRUE(lineBreakStateMachine.addAscii("LF"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("e"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("CR"));
    EXPECT_TRUE(lineBreakStateMachine.addAscii("LF"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("f"));
    EXPECT_TRUE(lineBreakStateMachine.addAscii("LF"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("CR"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("g"));
}
TEST(LineBreakStateMachine, LineBreakCrLf) {
    controller::LineBreakStateMachine lineBreakStateMachine{controller::LinebreakType::CRLF};
    EXPECT_FALSE(lineBreakStateMachine.addAscii("a"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("b"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("CR"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("d"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("LF"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("e"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("CR"));
    EXPECT_TRUE(lineBreakStateMachine.addAscii("LF"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("f"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("LF"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("CR"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("g"));
}
TEST(LineBreakStateMachine, LineBreakLfCr) {
    controller::LineBreakStateMachine lineBreakStateMachine{controller::LinebreakType::LFCR};
    EXPECT_FALSE(lineBreakStateMachine.addAscii("a"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("b"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("CR"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("d"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("LF"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("e"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("CR"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("LF"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("f"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("LF"));
    EXPECT_TRUE(lineBreakStateMachine.addAscii("CR"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("g"));
}
TEST(LineBreakStateMachine, LineBreakNone) {
    controller::LineBreakStateMachine lineBreakStateMachine{controller::LinebreakType::NONE};
    EXPECT_FALSE(lineBreakStateMachine.addAscii("a"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("b"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("CR"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("d"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("LF"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("e"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("CR"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("LF"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("f"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("LF"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("CR"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("g"));
}

TEST(LineBreakStateMachine, CrLfExtended) {
    controller::LineBreakStateMachine lineBreakStateMachine{controller::LinebreakType::CRLF};
    EXPECT_FALSE(lineBreakStateMachine.addAscii("LF"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("LF"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("CR"));
    EXPECT_TRUE(lineBreakStateMachine.addAscii("LF"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("a"));
    EXPECT_FALSE(lineBreakStateMachine.addAscii("a"));
}

TEST(LineBreakStateMachine, SetLineBreak) {
    controller::LineBreakStateMachine lineBreakStateMachine{controller::LinebreakType::NONE};
    EXPECT_FALSE(lineBreakStateMachine.addAscii("LF"));
    EXPECT_NO_THROW(lineBreakStateMachine.setLinebreak(controller::LinebreakType::LF));
    EXPECT_TRUE(lineBreakStateMachine.addAscii("LF"));
}

TEST(LineBreakStateMachine, Reset) {
    controller::LineBreakStateMachine lineBreakStateMachine{controller::LinebreakType::CRLF};
    EXPECT_FALSE(lineBreakStateMachine.addAscii("CR"));
    EXPECT_NO_THROW(lineBreakStateMachine.reset());
    EXPECT_FALSE(lineBreakStateMachine.addAscii("LF"));
}
