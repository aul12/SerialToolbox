/**
 * @file SerialProxy.cpp
 * @author paul
 * @date 31.12.18
 * @brief Definition of a wrapper between the raw interface and the controller
 */

#include "SerialProxy.hpp"

#include <sstream>
#include <bitset>

namespace controller {

    SerialProxy::SerialProxy(const std::shared_ptr<util::serial::Interface> &interface)
        : receiveListener{}, interface{interface} {
        interface->registerReceiveCallback(std::bind(&SerialProxy::readCallback, this, std::placeholders::_1));

        specialAsciiCharacters.insert({0, "NUL"});
        specialAsciiCharacters.insert({1, "SOH"});
        specialAsciiCharacters.insert({2, "STX"});
        specialAsciiCharacters.insert({3, "ETX"});
        specialAsciiCharacters.insert({4, "EOT"});
        specialAsciiCharacters.insert({5, "ENQ"});
        specialAsciiCharacters.insert({6, "ACK"});
        specialAsciiCharacters.insert({7, "BEL"});
        specialAsciiCharacters.insert({8, "BS"});
        specialAsciiCharacters.insert({9, "TAB"});
        specialAsciiCharacters.insert({10, "LF"});
        specialAsciiCharacters.insert({11, "VT"});
        specialAsciiCharacters.insert({12, "FF"});
        specialAsciiCharacters.insert({13, "CR"});
        specialAsciiCharacters.insert({14, "SO"});
        specialAsciiCharacters.insert({15, "SI"});
        specialAsciiCharacters.insert({16, "DLE"});
        specialAsciiCharacters.insert({17, "DC1"});
        specialAsciiCharacters.insert({18, "DC2"});
        specialAsciiCharacters.insert({19, "DC3"});
        specialAsciiCharacters.insert({20, "DC4"});
        specialAsciiCharacters.insert({21, "NAK"});
        specialAsciiCharacters.insert({22, "SYN"});
        specialAsciiCharacters.insert({23, "ETB"});
        specialAsciiCharacters.insert({24, "CAN"});
        specialAsciiCharacters.insert({25, "EM"});
        specialAsciiCharacters.insert({26, "SUB"});
        specialAsciiCharacters.insert({27, "ESC"});
        specialAsciiCharacters.insert({28, "FS"});
        specialAsciiCharacters.insert({29, "GS"});
        specialAsciiCharacters.insert({30, "RS"});
        specialAsciiCharacters.insert({31, "US"});
        specialAsciiCharacters.insert({127, "DEL"});
    }

    auto SerialProxy::send(const std::vector<std::string> &bytes, const Representation representation)
        -> std::deque<Representations> {
        std::vector<uint8_t> buf;

        std::transform(bytes.begin(), bytes.end(), std::back_inserter(buf),
                       [&representation, this] (const auto &b) { return convertToByte(b, representation); });

        interface->send(buf.begin(), buf.end());

        std::deque<Representations> representations;
        std::transform(buf.begin(), buf.end(), std::back_inserter(representations),
                std::bind(&SerialProxy::convertToRepresentations, this, std::placeholders::_1));

        return representations;
    }

    void SerialProxy::readCallback(const std::vector<uint8_t> &data) {
        std::deque<Representations> ret;
        std::transform(data.begin(), data.end(), std::back_inserter(ret),
                std::bind(&SerialProxy::convertToRepresentations, this, std::placeholders::_1));
        receiveListener(ret);
    }

    auto SerialProxy::convertToByte(const std::string &string, Representation representation) -> uint8_t {
        int base = 0;
        switch (representation) {
            case Representation::ASCII:
                if (string.length() == 1) {
                    return static_cast<uint8_t>(string.at(0));
                } else {
                    auto opt = specialAsciiCharactersReverseLookup(string);
                    if (opt.has_value()) {
                        return opt.value();
                    } else {
                        throw std::runtime_error("Not valid ascii");
                    }
                }
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

        if (specialAsciiCharacters.find(data) != specialAsciiCharacters.end()) {
            representations.ascii = specialAsciiCharacters.find(data)->second;
        } else {
            representations.ascii = static_cast<char>(data);
        }
        return representations;
    }

    auto SerialProxy::specialAsciiCharactersReverseLookup(const std::string &value) const -> std::optional<uint8_t> {
        for (const auto &item : this->specialAsciiCharacters) {
            if (item.second == value) {
                return item.first;
            }
        }
        return std::nullopt;
    }
}
