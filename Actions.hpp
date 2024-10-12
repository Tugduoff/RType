/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Actions.hpp file
*/

#ifndef ACTIONS_HPP_
    #define ACTIONS_HPP_

    #include <unordered_map>
    #include <string>
    #include <iostream>
    #include <iosfwd>

enum class Action {
    FORWARD, BACKWARD, LEFT, RIGHT,
    ACTION1, ACTION2, ACTION3, ACTION4,
    ACTION5, ACTION6, ACTION7, ACTION8,
    ACTION9, ACTION10,
    UNKNOWN
};

const std::unordered_map<std::string, Action> strToAction = {
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
    {"ACTION10", Action::ACTION10},
    {"UNKNOWN", Action::UNKNOWN}
};

/**
 * @brief Converts a string to an Action enum value.
 * 
 * @param actionString The string to convert to an Action enum value.
 * 
 * @return The Action enum value.
 */
Action stringToAction(const std::string &actionString);

/**
 * @brief Converts an Action enum value to a string.
 * 
 * @param os The output stream to write to.
 * 
 * @param action The Action enum value to convert to a string.
 */
std::ostream &operator<<(std::ostream &os, Action action);

#endif // ACTIONS_HPP_
