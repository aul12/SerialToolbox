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
