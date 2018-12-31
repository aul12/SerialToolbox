/**
 * @file SerialProxy.cpp
 * @author paul
 * @date 31.12.18
 * @brief SerialProxy @TODO
 */

#include "SerialProxy.hpp"

#include <sstream>
#include <bitset>

namespace controller {

    SerialProxy::SerialProxy(const std::shared_ptr<util::serial::Interface> &interface)
        : receiveListener{}, interface{interface} {

        interface->registerReceiveCallback(std::bind(&SerialProxy::readCallback, this, std::placeholders::_1));
    }

    void SerialProxy::send(std::vector<std::string> bytes, Representation representation) {

    }

    void SerialProxy::readCallback(std::vector<uint8_t> data) {
        std::deque<std::unordered_map<Representation, std::string>> ret;
        for (const auto &dat : data) {
            std::stringstream stringstream;
            Representations representations{};

            stringstream << std::hex << static_cast<int>(dat);
            representations.hex = stringstream.str();
            stringstream.clear();

            stringstream << std::dec << static_cast<int>(dat);
            representations.dec = stringstream.str();
            stringstream.clear();

            std::bitset<8> bitset(static_cast<unsigned long>(dat));
            representations.bin = bitset.to_string();

            representations.ascii = static_cast<char>(dat);
        }
    }
}
