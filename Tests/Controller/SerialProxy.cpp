#include <gtest/gtest.h>

#include "../Util/Serial/InterfaceImpl.hpp"
#include "Controller/SerialProxy.hpp"

TEST(SerialProxy, Test) {
    auto iface = std::make_shared<InterfaceImpl>();
}
