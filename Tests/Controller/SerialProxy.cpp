#include "Controller/SerialProxy.hpp"

#include <gtest/gtest.h>

#include "Tests/Util/Serial/InterfaceMock.hpp"

TEST(SerialProxy, SendBin) {
    auto iface = std::make_shared<InterfaceMock>();
    controller::SerialProxy serialProxy{iface};

    std::vector<uint8_t> bin{0, 1, 2, 3, 2};

    EXPECT_NO_THROW(serialProxy.send({"00", "01", "10", "11", "10"}, controller::Representation::BIN));
    EXPECT_EQ(iface->buffer, bin);
}

TEST(SerialProxy, SendHex) {
    auto iface = std::make_shared<InterfaceMock>();
    controller::SerialProxy serialProxy{iface};

    std::vector<uint8_t> hex{0, 1, 16, 17, 16};

    EXPECT_NO_THROW(serialProxy.send({"00", "01", "10", "11", "10"}, controller::Representation::HEX));
    EXPECT_EQ(iface->buffer, hex);
}

TEST(SerialProxy, SendDec) {
    auto iface = std::make_shared<InterfaceMock>();
    controller::SerialProxy serialProxy{iface};

    std::vector<uint8_t> dec{0, 1, 10, 11, 10};

    EXPECT_NO_THROW(serialProxy.send({"00", "01", "10", "11", "10"}, controller::Representation::DEC));
    EXPECT_EQ(iface->buffer, dec);
}

TEST(SerialProxy, SendAscii) {
    auto iface = std::make_shared<InterfaceMock>();
    controller::SerialProxy serialProxy{iface};

    std::vector<uint8_t> dec{97, 98, 99, 100, 101};

    EXPECT_NO_THROW(serialProxy.send({"a", "b", "c", "d", "e"}, controller::Representation::ASCII));
    EXPECT_EQ(iface->buffer, dec);
}

TEST(SerialProxy, SendAsciiSpecial) {
    auto iface = std::make_shared<InterfaceMock>();
    controller::SerialProxy serialProxy{iface};

    std::vector<uint8_t> dec{0, 7, 10, 13};

    EXPECT_NO_THROW(serialProxy.send({"NUL", "BEL", "LF", "CR"}, controller::Representation::ASCII));
    EXPECT_EQ(iface->buffer, dec);
}

TEST(SerialProxy, SendBinFail) {
    auto iface = std::make_shared<InterfaceMock>();
    controller::SerialProxy serialProxy{iface};
    EXPECT_THROW(serialProxy.send({""}, controller::Representation::BIN), std::runtime_error);
    EXPECT_THROW(serialProxy.send({"-1"}, controller::Representation::BIN), std::runtime_error);
    EXPECT_THROW(serialProxy.send({"asas"}, controller::Representation::BIN), std::runtime_error);
    EXPECT_THROW(serialProxy.send({"111111111"}, controller::Representation::BIN), std::runtime_error);
    EXPECT_NO_THROW(serialProxy.send({"0"}, controller::Representation::BIN));
    EXPECT_NO_THROW(serialProxy.send({"11111111"}, controller::Representation::BIN));
}

TEST(SerialProxy, SendHexFail) {
    auto iface = std::make_shared<InterfaceMock>();
    controller::SerialProxy serialProxy{iface};
    EXPECT_THROW(serialProxy.send({""}, controller::Representation::HEX), std::runtime_error);
    EXPECT_THROW(serialProxy.send({"-AF"}, controller::Representation::HEX), std::runtime_error);
    EXPECT_THROW(serialProxy.send({"GHIJ"}, controller::Representation::HEX), std::runtime_error);
    EXPECT_THROW(serialProxy.send({"100"}, controller::Representation::HEX), std::runtime_error);
    EXPECT_NO_THROW(serialProxy.send({"FF"}, controller::Representation::HEX));
    EXPECT_NO_THROW(serialProxy.send({"0"}, controller::Representation::HEX));
}

TEST(SerialProxy, SendDecFail) {
    auto iface = std::make_shared<InterfaceMock>();
    controller::SerialProxy serialProxy{iface};
    EXPECT_THROW(serialProxy.send({""}, controller::Representation::DEC), std::runtime_error);
    EXPECT_THROW(serialProxy.send({"-1"}, controller::Representation::DEC), std::runtime_error);
    EXPECT_THROW(serialProxy.send({"GHIJ"}, controller::Representation::DEC), std::runtime_error);
    EXPECT_THROW(serialProxy.send({"256"}, controller::Representation::DEC), std::runtime_error);
    EXPECT_NO_THROW(serialProxy.send({"255"}, controller::Representation::DEC));
    EXPECT_NO_THROW(serialProxy.send({"0"}, controller::Representation::DEC));
}

TEST(SerialProxy, SendAsciiFail) {
    auto iface = std::make_shared<InterfaceMock>();
    controller::SerialProxy serialProxy{iface};
    EXPECT_THROW(serialProxy.send({""}, controller::Representation::ASCII), std::runtime_error);
    EXPECT_THROW(serialProxy.send({"AA"}, controller::Representation::ASCII), std::runtime_error);
    EXPECT_NO_THROW(serialProxy.send({"a"}, controller::Representation::ASCII));
    EXPECT_NO_THROW(serialProxy.send({{static_cast<char>(0)}}, controller::Representation::ASCII));
    EXPECT_NO_THROW(serialProxy.send({{static_cast<char>(255)}}, controller::Representation::ASCII));
}

TEST(SerialProxy, Receive) {
    auto iface = std::make_shared<InterfaceMock>();
    controller::SerialProxy serialProxy{iface};
    bool called = false;

    decltype(serialProxy.receiveListener)::type f = [&called](std::deque<controller::Representations>) {
        called = true;
    };

    EXPECT_FALSE(called);
    EXPECT_NO_THROW(serialProxy.receiveListener(f));
    EXPECT_NO_THROW(iface->doCallback({}));
    EXPECT_TRUE(called);
}

TEST(SerialProxy, ReceiveBin) {
    auto iface = std::make_shared<InterfaceMock>();
    controller::SerialProxy serialProxy{iface};
    std::deque<controller::Representations> recv;
    std::vector<uint8_t> send{0, 1, 2, 3, 127, 255};
    std::deque<std::string> recvBin{"00000000", "00000001", "00000010", "00000011", "01111111", "11111111"};

    decltype(serialProxy.receiveListener)::type f = [&recv](std::deque<controller::Representations> data) {
        recv = data;
    };

    EXPECT_NO_THROW(serialProxy.receiveListener(f));
    EXPECT_NO_THROW(iface->doCallback(send));
    EXPECT_TRUE(std::equal(recvBin.begin(), recvBin.end(), recv.begin(),
                           [](const auto &rb, const auto &r) { return rb == r.bin; }));
}

TEST(SerialProxy, ReceiveHex) {
    auto iface = std::make_shared<InterfaceMock>();
    controller::SerialProxy serialProxy{iface};
    std::deque<controller::Representations> recv;
    std::vector<uint8_t> send{0, 1, 2, 3, 127, 255};
    std::deque<std::string> recvHex{"0", "1", "2", "3", "7F", "FF"};

    decltype(serialProxy.receiveListener)::type f = [&recv](std::deque<controller::Representations> data) {
        recv = data;
    };

    EXPECT_NO_THROW(serialProxy.receiveListener(f));
    EXPECT_NO_THROW(iface->doCallback(send));
    EXPECT_TRUE(std::equal(recvHex.begin(), recvHex.end(), recv.begin(),
                           [](const auto &rb, const auto &r) { return rb == r.hex; }));
}

TEST(SerialProxy, ReceiveDec) {
    auto iface = std::make_shared<InterfaceMock>();
    controller::SerialProxy serialProxy{iface};
    std::deque<controller::Representations> recv;
    std::vector<uint8_t> send{0, 1, 2, 3, 127, 255};
    std::deque<std::string> recvDec{"0", "1", "2", "3", "127", "255"};

    decltype(serialProxy.receiveListener)::type f = [&recv](std::deque<controller::Representations> data) {
        recv = data;
    };

    EXPECT_NO_THROW(serialProxy.receiveListener(f));
    EXPECT_NO_THROW(iface->doCallback(send));
    EXPECT_TRUE(std::equal(recvDec.begin(), recvDec.end(), recv.begin(),
                           [](const auto &rb, const auto &r) { return rb == r.dec; }));
}

TEST(SerialProxy, ReceiveAscii) {
    auto iface = std::make_shared<InterfaceMock>();
    controller::SerialProxy serialProxy{iface};
    std::deque<controller::Representations> recv;
    std::vector<uint8_t> send{0, 1, 2, 3, 'a', ';', 127, 255};
    std::deque<std::string> recvAscii{
            {"NUL"}, {"SOH"}, {"STX"}, {"ETX"}, "a", ";", {"DEL"}, {static_cast<char>(255)},
    };

    decltype(serialProxy.receiveListener)::type f = [&recv](std::deque<controller::Representations> data) {
        recv = data;
    };

    EXPECT_NO_THROW(serialProxy.receiveListener(f));
    EXPECT_NO_THROW(iface->doCallback(send));
    EXPECT_TRUE(std::equal(recvAscii.begin(), recvAscii.end(), recv.begin(),
                           [](const auto &rb, const auto &r) { return rb == r.ascii; }));
}
