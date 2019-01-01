#include <gtest/gtest.h>

#include "Util/Listener.hpp"

TEST(Listener, AddGetSubscribed) {
    util::Listener<> listener{};

    util::Listener<>::type f = [](){};

    EXPECT_EQ(listener.getSubscribed(), 0);
    EXPECT_NO_THROW(listener(f));
    EXPECT_EQ(listener.getSubscribed(), 1);
    EXPECT_NO_THROW(listener(f));
    EXPECT_EQ(listener.getSubscribed(), 2);
}

TEST(Listener, AddCall) {
    util::Listener<> listener{};
    volatile int i = 0;

    util::Listener<>::type f = [&i](){i++;};

    EXPECT_NO_THROW(listener(f));
    EXPECT_NO_THROW(listener());
    EXPECT_EQ(i, 1);
    EXPECT_NO_THROW(listener(f));
    EXPECT_NO_THROW(listener());
    EXPECT_EQ(i, 3);
}

TEST(Listner, Args) {
    util::Listener<int> listener{};
    volatile int i = 0;

    util::Listener<int>::type f = [&i](int a){i+=a;};

    EXPECT_NO_THROW(listener(f));
    EXPECT_NO_THROW(listener(2));
    EXPECT_EQ(i, 2);
    EXPECT_NO_THROW(listener(f));
    EXPECT_NO_THROW(listener(2));
    EXPECT_EQ(i, 6);
}