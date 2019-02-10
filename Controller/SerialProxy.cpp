/**
 * @file SerialProxy.cpp
 * @author paul
 * @date 31.12.18
 * @brief SerialProxy @TODO
 */

#include "SerialProxy.hpp"

#include <sstream>
#include <bitset>
#include <cassert>

namespace controller {

    SerialProxy::SerialProxy(const std::shared_ptr<util::serial::Interface> &interface)
        : receiveListener{}, interface{interface} {

        interface->registerReceiveCallback(std::bind(&SerialProxy::readCallback, this, std::placeholders::_1));
    }

    auto SerialProxy::send(std::vector<std::string> bytes, Representation representation)
        -> std::deque<Representations> {
        std::vector<uint8_t> buf;

        std::transform(bytes.begin(), bytes.end(), std::back_inserter(buf),
                       [&representation] (const auto &b) { return convertToByte(b, representation); });

        interface->send(buf.begin(), buf.end());

        std::deque<Representations> representations;
        std::transform(buf.begin(), buf.end(), std::back_inserter(representations), convertToRepresentations);

        return representations;
    }

    void SerialProxy::readCallback(std::vector<uint8_t> data) {
        std::deque<Representations> ret;
        std::transform(data.begin(), data.end(), std::back_inserter(ret), convertToRepresentations);
        receiveListener(ret);
    }

    auto SerialProxy::convertToByte(std::string string, Representation representation) -> uint8_t {
        int base = 0;
        switch (representation) {
            case Representation::ASCII:
                if (string.length() != 1) {
                    throw std::runtime_error("Not valid ascii");
                }
                return static_cast<uint8_t>(string.at(0));
            case Representation::HEX:
                base = 16;
                break;
            case Representation::DEC:
                base = 10;
                break;
            case Representation::BIN:
                base = 2;
                break;
        }

        int val = 0;
        try {
            val = std::stoi(string, nullptr, base);
        } catch (std::invalid_argument &e) {
            throw std::runtime_error(e.what());
        }

        if (val < 0 || val > 255) {
            throw std::runtime_error("Not one byte");
        }

        return static_cast<uint8_t>(val);
    }

    auto SerialProxy::convertToRepresentations(uint8_t data) -> Representations {
        std::stringstream stringstream;
        Representations representations{};

        stringstream << std::hex << std::uppercase << static_cast<int>(data);
        representations.hex = stringstream.str();

        stringstream.str({});
        stringstream << std::dec << static_cast<int>(data);
        representations.dec = stringstream.str();

        std::bitset<8> bitset(static_cast<unsigned long>(data));
        representations.bin = bitset.to_string();

        representations.ascii = static_cast<char>(data);
        return representations;
    }
}
