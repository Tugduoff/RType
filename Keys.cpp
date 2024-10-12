/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Keys.cpp file
*/

#include "Keys.hpp"

const std::unordered_map<std::string, Key> strToKey = {
    {"A", Key::A}, {"B", Key::B}, {"C", Key::C}, {"D", Key::D},
    {"E", Key::E}, {"F", Key::F}, {"G", Key::G}, {"H", Key::H},
    {"I", Key::I}, {"J", Key::J}, {"K", Key::K}, {"L", Key::L},
    {"M", Key::M}, {"N", Key::N}, {"O", Key::O}, {"P", Key::P},
    {"Q", Key::Q}, {"R", Key::R}, {"S", Key::S}, {"T", Key::T},
    {"U", Key::U}, {"V", Key::V}, {"W", Key::W}, {"X", Key::X},
    {"Y", Key::Y}, {"Z", Key::Z},
    {"UP", Key::UP_KEY}, {"DOWN", Key::DOWN_KEY}, {"LEFT", Key::LEFT_KEY}, {"RIGHT", Key::RIGHT_KEY},
    {"SPACE", Key::SPACE}, {"ENTER", Key::ENTER}, {"ESCAPE", Key::ESCAPE}, {"DELETE", Key::DELETE},
    {"BACKSPACE", Key::BACKSPACE},
    {"NUM_0", Key::NUM_0}, {"NUM_1", Key::NUM_1}, {"NUM_2", Key::NUM_2}, {"NUM_3", Key::NUM_3},
    {"NUM_4", Key::NUM_4}, {"NUM_5", Key::NUM_5}, {"NUM_6", Key::NUM_6}, {"NUM_7", Key::NUM_7},
    {"NUM_8", Key::NUM_8}, {"NUM_9", Key::NUM_9},
    {"LEFT_CLICK", Key::LEFT_CLICK}, {"RIGHT_CLICK", Key::RIGHT_CLICK}, {"MIDDLE_CLICK", Key::MIDDLE_CLICK}
};

Key stringToKey(const std::string &keyString) {
    auto it = strToKey.find(keyString);
    if (it != strToKey.end()) {
        return it->second;
    } else {
        return Key::UNKNOWN;
    }
}

std::ostream &operator<<(std::ostream &os, Key key)
{
    for (auto &pair : strToKey) {
        if (pair.second == key) {
            os << pair.first;
            return os;
        }
    }
}
