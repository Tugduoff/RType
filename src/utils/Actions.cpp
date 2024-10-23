/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Actions.cpp file
*/

#include "Actions.hpp"

Action stringToAction(const std::string &actionString) {
    auto it = strToAction.find(actionString);
    if (it != strToAction.end()) {
        return it->second;
    } else {
        return Action::UNKNOWN;
    }
}

std::ostream &operator<<(std::ostream &os, Action action)
{
    for (auto &pair : strToAction) {
        if (pair.second == action) {
            os << pair.first;
            return os;
        }
    }
    return os;
}

std::string actionToString(const Action &action)
{
    for (const auto &pair : strToAction) {
        if (pair.second == action) {
            return pair.first;  // Return the string by value
        }
    }
    return "UNKNOWN";  // Return "UNKNOWN" as a value
}
