#include <gtest/gtest.h>

#include "InterfaceImpl.hpp"

TEST(Interface, Send) {
    InterfaceImpl iface{};
    std::vector<uint8_t> a{1,2,3};
    std::vector<char> b{'a','b','c'};

    EXPECT_NO_THROW(iface.send(a.begin(), a.end()));
    EXPECT_TRUE(std::equal(a.begin(), a.end(), iface.buffer.begin()));
    EXPECT_FALSE(std::equal(b.begin(), b.end(), iface.buffer.begin()));

    EXPECT_NO_THROW(iface.send(b.begin(), b.end()));
    EXPECT_FALSE(std::equal(a.begin(), a.end(), iface.buffer.begin()));
    EXPECT_TRUE(std::equal(b.begin(), b.end(), iface.buffer.begin()));
}

TEST(Interface, OrderCheck) {
    InterfaceImpl iface{};
    std::vector<uint8_t> a{1,2,3};
    std::vector<char> b{'a','b','c'};

    EXPECT_NO_FATAL_FAILURE(iface.send(a.begin(), a.end()));
    EXPECT_NO_FATAL_FAILURE(iface.send(a.begin(), a.end()));
    EXPECT_DEATH(iface.send(a.end(), a.begin()), "");
    EXPECT_DEATH(iface.send(b.end(), b.begin()), "");
}

TEST(Interface, CallbackNotAvailable) {
    InterfaceImpl iface{};
    EXPECT_NO_THROW(iface.doCallback({1,2,3}));
    EXPECT_NO_THROW(iface.doCallback({1,2}));
}

TEST(Interface, DuplicateCallback) {
    InterfaceImpl iface{};

    std::function<void(std::vector<uint8_t>)> f = [](std::vector<uint8_t>){};

    EXPECT_NO_THROW(iface.registerReceiveCallback(f));
    EXPECT_THROW(iface.registerReceiveCallback(f), std::logic_error);
}

TEST(Interface, Callback) {
    InterfaceImpl iface{};
    std::vector<uint8_t> a{1,2,3};
    std::vector<uint8_t> b{4,5,6};
    std::vector<uint8_t> v;

    std::function<void(std::vector<uint8_t>)> f = [&v](std::vector<uint8_t> data) {
        v = data;
    };

    EXPECT_TRUE(v.empty());
    EXPECT_NO_THROW(iface.registerReceiveCallback(f));
    EXPECT_NO_THROW(iface.doCallback(a));
    EXPECT_TRUE(std::equal(a.begin(), a.end(), v.begin()));
    EXPECT_FALSE(std::equal(b.begin(), b.end(), v.begin()));
    EXPECT_NO_THROW(iface.doCallback(b));
    EXPECT_TRUE(std::equal(b.begin(), b.end(), v.begin()));
    EXPECT_FALSE(std::equal(a.begin(), a.end(), v.begin()));
}