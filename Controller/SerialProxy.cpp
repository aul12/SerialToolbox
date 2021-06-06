/**
 * @file SerialProxy.cpp
 * @author paul
 * @date 31.12.18
 * @brief Definition of a wrapper between the raw interface and the controller
 */

#include "SerialProxy.hpp"

#include <bitset>
#include <sstream>

namespace controller {

    SerialProxy::SerialProxy(const std::shared_ptr<util::serial::Interface> &interface) :
        receiveListener{},
        interface{interface} {
        interface->registerReceiveCallback(std::bind(&SerialProxy::readCallback, this, std::placeholders::_1));

        specialAsciiCharacters.emplace(0, "NUL");
        specialAsciiCharacters.emplace(1, "SOH");
        specialAsciiCharacters.emplace(2, "STX");
        specialAsciiCharacters.emplace(3, "ETX");
        specialAsciiCharacters.emplace(4, "EOT");
        specialAsciiCharacters.emplace(5, "ENQ");
        specialAsciiCharacters.emplace(6, "ACK");
        specialAsciiCharacters.emplace(7, "BEL");
        specialAsciiCharacters.emplace(8, "BS");
        specialAsciiCharacters.emplace(9, "TAB");
        specialAsciiCharacters.emplace(10, "LF");
        specialAsciiCharacters.emplace(11, "VT");
        specialAsciiCharacters.emplace(12, "FF");
        specialAsciiCharacters.emplace(13, "CR");
        specialAsciiCharacters.emplace(14, "SO");
        specialAsciiCharacters.emplace(15, "SI");
        specialAsciiCharacters.emplace(16, "DLE");
        specialAsciiCharacters.emplace(17, "DC1");
        specialAsciiCharacters.emplace(18, "DC2");
        specialAsciiCharacters.emplace(19, "DC3");
        specialAsciiCharacters.emplace(20, "DC4");
        specialAsciiCharacters.emplace(21, "NAK");
        specialAsciiCharacters.emplace(22, "SYN");
        specialAsciiCharacters.emplace(23, "ETB");
        specialAsciiCharacters.emplace(24, "CAN");
        specialAsciiCharacters.emplace(25, "EM");
        specialAsciiCharacters.emplace(26, "SUB");
        specialAsciiCharacters.emplace(27, "ESC");
        specialAsciiCharacters.emplace(28, "FS");
        specialAsciiCharacters.emplace(29, "GS");
        specialAsciiCharacters.emplace(30, "RS");
        specialAsciiCharacters.emplace(31, "US");
        specialAsciiCharacters.emplace(127, "DEL");
    }

    auto SerialProxy::send(const std::vector<std::string> &bytes, const Representation representation)
            -> std::deque<Representations> {
        std::vector<uint8_t> buf;

        std::transform(bytes.begin(), bytes.end(), std::back_inserter(buf),
                       [&representation, this](const std::string &b) { return convertToByte(b, representation); });

        interface->send(buf.begin(), buf.end());

        std::deque<Representations> representations;
        std::transform(buf.begin(), buf.end(), std::back_inserter(representations),
                       [this](uint8_t data) { return convertToRepresentations(data); });

        return representations;
    }

    void SerialProxy::readCallback(const std::vector<uint8_t> &data) {
        std::deque<Representations> ret;
        std::transform(data.begin(), data.end(), std::back_inserter(ret),
                       [this](uint8_t data) { return convertToRepresentations(data); });
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
                    }
                    throw std::runtime_error("Not valid ascii");
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
} // namespace controller
