#include <gtest/gtest.h>

#include "../Util/Serial/InterfaceImpl.hpp"
#include "Controller/SerialProxy.hpp"

TEST(SerialProxy, SendBin) {
    auto iface = std::make_shared<InterfaceImpl>();
    controller::SerialProxy serialProxy{iface};

    std::vector<uint8_t> bin{0,1,2,3,2};

    EXPECT_NO_THROW(serialProxy.send({"00", "01", "10", "11", "10"}, controller::Representation::BIN));
    EXPECT_EQ(iface->buffer, bin);
}

TEST(SerialProxy, SendHex) {
    auto iface = std::make_shared<InterfaceImpl>();
    controller::SerialProxy serialProxy{iface};

    std::vector<uint8_t> hex{0,1,16,17,16};

    EXPECT_NO_THROW(serialProxy.send({"00", "01", "10", "11", "10"}, controller::Representation::HEX));
    EXPECT_EQ(iface->buffer, hex);
}

TEST(SerialProxy, SendDec) {
    auto iface = std::make_shared<InterfaceImpl>();
    controller::SerialProxy serialProxy{iface};

    std::vector<uint8_t> dec{0,1,10,11,10};

    EXPECT_NO_THROW(serialProxy.send({"00", "01", "10", "11", "10"}, controller::Representation::DEC));
    EXPECT_EQ(iface->buffer, dec);
}

TEST(SerialProxy, SendBinFail) {
    auto iface = std::make_shared<InterfaceImpl>();
    controller::SerialProxy serialProxy{iface};
    EXPECT_THROW(serialProxy.send({""}, controller::Representation::BIN), std::runtime_error);
    EXPECT_THROW(serialProxy.send({"-1"}, controller::Representation::BIN), std::runtime_error);
    EXPECT_THROW(serialProxy.send({"asas"}, controller::Representation::BIN), std::runtime_error);
    EXPECT_THROW(serialProxy.send({"111111111"}, controller::Representation::BIN), std::runtime_error);
    EXPECT_NO_THROW(serialProxy.send({"0"}, controller::Representation::BIN));
    EXPECT_NO_THROW(serialProxy.send({"11111111"}, controller::Representation::BIN));
}

TEST(SerialProxy, SendHexFail) {
    auto iface = std::make_shared<InterfaceImpl>();
    controller::SerialProxy serialProxy{iface};
    EXPECT_THROW(serialProxy.send({""}, controller::Representation::HEX), std::runtime_error);
    EXPECT_THROW(serialProxy.send({"-AF"}, controller::Representation::HEX), std::runtime_error);
    EXPECT_THROW(serialProxy.send({"GHIJ"}, controller::Representation::HEX), std::runtime_error);
    EXPECT_THROW(serialProxy.send({"100"}, controller::Representation::HEX), std::runtime_error);
    EXPECT_NO_THROW(serialProxy.send({"FF"}, controller::Representation::HEX));
    EXPECT_NO_THROW(serialProxy.send({"0"}, controller::Representation::HEX));
}

TEST(SerialProxy, SendDecFail) {
    auto iface = std::make_shared<InterfaceImpl>();
    controller::SerialProxy serialProxy{iface};
    EXPECT_THROW(serialProxy.send({""}, controller::Representation::DEC), std::runtime_error);
    EXPECT_THROW(serialProxy.send({"-1"}, controller::Representation::DEC), std::runtime_error);
    EXPECT_THROW(serialProxy.send({"GHIJ"}, controller::Representation::DEC), std::runtime_error);
    EXPECT_THROW(serialProxy.send({"256"}, controller::Representation::DEC), std::runtime_error);
    EXPECT_NO_THROW(serialProxy.send({"255"}, controller::Representation::DEC));
    EXPECT_NO_THROW(serialProxy.send({"0"}, controller::Representation::DEC));
}

TEST(SerialProxy, SendAsciiFail) {
    auto iface = std::make_shared<InterfaceImpl>();
    controller::SerialProxy serialProxy{iface};
    EXPECT_THROW(serialProxy.send({""}, controller::Representation::ASCII), std::runtime_error);
    EXPECT_THROW(serialProxy.send({"AA"}, controller::Representation::ASCII), std::runtime_error);
    EXPECT_NO_THROW(serialProxy.send({"a"}, controller::Representation::ASCII));
    EXPECT_NO_THROW(serialProxy.send({{static_cast<char>(0)}}, controller::Representation::ASCII));
    EXPECT_NO_THROW(serialProxy.send({{static_cast<char>(255)}}, controller::Representation::ASCII));
}

TEST(SerialProxy, Receive) {
    auto iface = std::make_shared<InterfaceImpl>();
    controller::SerialProxy serialProxy{iface};
    bool called = false;

    decltype(serialProxy.receiveListener)::type f = [&called](std::deque<controller::Representations> repr){
        called = true;
    };

    EXPECT_NO_THROW(serialProxy.receiveListener(f));
    EXPECT_NO_THROW(iface->doCallback({}));

    EXPECT_TRUE(called);
}
