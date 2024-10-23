/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Keys.cpp file
*/

#include "Keys.hpp"

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
    return os;
}
