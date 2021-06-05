#include <gtest/gtest.h>

#include "InterfaceMock.hpp"

TEST(Interface, Send) {
    InterfaceMock iface{};
    std::vector<uint8_t> a{1, 2, 3};
    std::vector<char> b{'a', 'b', 'c'};

    EXPECT_NO_THROW(iface.send(a.begin(), a.end()));
    EXPECT_TRUE(std::equal(a.begin(), a.end(), iface.buffer.begin()));
    EXPECT_FALSE(std::equal(b.begin(), b.end(), iface.buffer.begin()));

    EXPECT_NO_THROW(iface.send(b.begin(), b.end()));
    EXPECT_FALSE(std::equal(a.begin(), a.end(), iface.buffer.begin()));
    EXPECT_TRUE(std::equal(b.begin(), b.end(), iface.buffer.begin()));
}

TEST(Interface, OrderCheck) {
    InterfaceMock iface{};
    std::vector<uint8_t> a{1, 2, 3};
    std::vector<char> b{'a', 'b', 'c'};

    EXPECT_NO_THROW(iface.send(a.begin(), a.end()));
    EXPECT_NO_THROW(iface.send(b.begin(), b.end()));
    EXPECT_THROW(iface.send(a.end(), a.begin()), std::runtime_error);
    EXPECT_THROW(iface.send(b.end(), b.begin()), std::runtime_error);
}

TEST(Interface, CallbackNotAvailable) {
    InterfaceMock iface{};
    EXPECT_NO_THROW(iface.doCallback({1, 2, 3}));
    EXPECT_NO_THROW(iface.doCallback({1, 2}));
}

TEST(Interface, DuplicateCallback) {
    InterfaceMock iface{};

    std::function<void(std::vector<uint8_t>)> f = [](std::vector<uint8_t>) {};

    EXPECT_NO_THROW(iface.registerReceiveCallback(f));
    EXPECT_THROW(iface.registerReceiveCallback(f), std::logic_error);
}

TEST(Interface, Callback) {
    InterfaceMock iface{};
    std::vector<uint8_t> a{1, 2, 3};
    std::vector<uint8_t> b{4, 5, 6};
    std::vector<uint8_t> v;

    std::function<void(std::vector<uint8_t>)> f = [&v](std::vector<uint8_t> data) { v = data; };

    EXPECT_TRUE(v.empty());
    EXPECT_NO_THROW(iface.registerReceiveCallback(f));
    EXPECT_NO_THROW(iface.doCallback(a));
    EXPECT_EQ(a, v);
    EXPECT_NE(b, v);
    EXPECT_NO_THROW(iface.doCallback(b));
    EXPECT_EQ(b, v);
    EXPECT_NE(a, v);
}
