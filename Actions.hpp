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

enum class Action : int {
    FORWARD,
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

extern const std::unordered_map<std::string, Action> strToAction;

/**
 * @brief Converts a string to an Action enum value.
 * 
 * @param actionString The string to convert to an Action enum value.
 * 
 * @return The Action enum value.
 */
Action stringToAction(const std::string &actionString);

std::ostream &operator<<(std::ostream &os, Action action);

#endif // ACTIONS_HPP
