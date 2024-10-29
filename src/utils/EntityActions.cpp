/*
** EPITECH PROJECT, 2024
** RType
** File description:
** EntityActions.cpp file
*/

#include "EntityActions.hpp"

EntityAction stringToEntityAction(const std::string &actionString) {
    auto it = strToEntityAction.find(actionString);
    if (it != strToEntityAction.end()) {
        return it->second;
    } else {
        return EntityAction::UNKNOWN;
    }
}

std::ostream &operator<<(std::ostream &os, EntityAction action)
{
    for (auto &pair : strToEntityAction) {
        if (pair.second == action) {
            os << pair.first;
            return os;
        }
    }
    return os;
}

std::string actionToString(const EntityAction &action)
{
    for (const auto &pair : strToEntityAction) {
        if (pair.second == action) {
            return pair.first;  // Return the string by value
        }
    }
    return "UNKNOWN";  // Return "UNKNOWN" as a value
}
