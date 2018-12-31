#include "gtest/gtest.h"

#include "Util/Listener.hpp"

TEST(Listener, Add) {
    util::Listener<> listener{};

    util::Listener<>::type f = [](){};

    EXPECT_EQ(listener.getSubscribed(), 0);
    EXPECT_NO_THROW(listener(f));
    EXPECT_EQ(listener.getSubscribed(), 1);
    EXPECT_NO_THROW(listener(f));
    EXPECT_EQ(listener.getSubscribed(), 2);
}
