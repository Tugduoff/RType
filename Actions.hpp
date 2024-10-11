/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Actions.hpp file
*/

#ifndef ACTIONS_HPP
    #define ACTIONS_HPP

    #include <unordered_map>
    #include <string>
    #include <stdexcept>

enum Action {
    FORWARD = 0,
    BACKWARD,
    LEFT,
    RIGHT,
    ACTION1,
    ACTION2,
    ACTION3,
    ACTION4,
    ACTION5,
    ACTION6,
    ACTION7,
    ACTION8,
    ACTION9,
    ACTION10
};

static const std::unordered_map<std::string, Action> strToAction = {
    {"FORWARD", Action::FORWARD},
    {"BACKWARD", Action::BACKWARD},
    {"LEFT", Action::LEFT},
    {"RIGHT", Action::RIGHT},
    {"ACTION1", Action::ACTION1},
    {"ACTION2", Action::ACTION2},
    {"ACTION3", Action::ACTION3},
    {"ACTION4", Action::ACTION4},
    {"ACTION5", Action::ACTION5},
    {"ACTION6", Action::ACTION6},
    {"ACTION7", Action::ACTION7},
    {"ACTION8", Action::ACTION8},
    {"ACTION9", Action::ACTION9},
    {"ACTION10", Action::ACTION10}
};

/**
 * @brief Converts a string to an Action enum value.
 * 
 * @param actionString The string to convert to an Action enum value.
 * 
 * @return The Action enum value.
 */
Action stringToAction(const std::string &actionString) {
    auto it = strToAction.find(actionString);
    if (it != strToAction.end()) {
        return it->second;
    } else {
        throw std::invalid_argument("Invalid action string: " + actionString);
    }
}

#endif // ACTIONS_HPP
