/*
** EPITECH PROJECT, 2024
** RType
** File description:
** EntityActions.hpp file
*/

#ifndef ENTITY_ACTIONS_HPP_
    #define ENTITY_ACTIONS_HPP_

    #include <unordered_map>
    #include <string>
    #include <iostream>
    #include <iosfwd>

enum class EntityAction {
    IDLE = 0,
    FORWARD, BACKWARD, LEFT, RIGHT,
    FORWARD_RIGHT, FORWARD_LEFT, BACKWARD_RIGHT, BACKWARD_LEFT,
    SHOOT_FORWARD, SHOOT_BACKWARD, SHOOT_LEFT, SHOOT_RIGHT,
    SHOOT_FORWARD_RIGHT, SHOOT_FORWARD_LEFT, SHOOT_BACKWARD_RIGHT, SHOOT_BACKWARD_LEFT,
    UNKNOWN
};

const std::unordered_map<std::string, EntityAction> strToEntityAction = {
    {"IDLE", EntityAction::IDLE},
    {"FORWARD", EntityAction::FORWARD},
    {"BACKWARD", EntityAction::BACKWARD},
    {"LEFT", EntityAction::LEFT},
    {"RIGHT", EntityAction::RIGHT},
    {"FORWARD_RIGHT", EntityAction::FORWARD_RIGHT},
    {"FORWARD_LEFT", EntityAction::FORWARD_LEFT},
    {"BACKWARD_RIGHT", EntityAction::BACKWARD_RIGHT},
    {"BACKWARD_LEFT", EntityAction::BACKWARD_LEFT},
    {"SHOOT_FORWARD", EntityAction::SHOOT_FORWARD},
    {"SHOOT_BACKWARD", EntityAction::SHOOT_BACKWARD},
    {"SHOOT_LEFT", EntityAction::SHOOT_LEFT},
    {"SHOOT_RIGHT", EntityAction::SHOOT_RIGHT},
    {"SHOOT_FORWARD_RIGHT", EntityAction::SHOOT_FORWARD_RIGHT},
    {"SHOOT_FORWARD_LEFT", EntityAction::SHOOT_FORWARD_LEFT},
    {"SHOOT_BACKWARD_RIGHT", EntityAction::SHOOT_BACKWARD_RIGHT},
    {"SHOOT_BACKWARD_LEFT", EntityAction::SHOOT_BACKWARD_LEFT},
    {"UNKNOWN", EntityAction::UNKNOWN}
};

/**
 * @brief Converts a string to an EntityAction enum value.
 * 
 * @param actionString The string to convert to an EntityAction enum value.
 * 
 * @return The EntityAction enum value.
 */
EntityAction stringToEntityAction(const std::string &actionString);

/**
 * @brief Converts an EntityAction enum value to a string.
 * 
 * @param action The EntityAction enum value to convert to a string.
 * 
 * @return The string representation of the EntityAction enum value.
 */
std::string actionToString(const EntityAction &action);

/**
 * @brief Converts an EntityAction enum value to a string.
 * 
 * @param os The output stream to write to.
 * 
 * @param action The EntityAction enum value to convert to a string.
 */
std::ostream &operator<<(std::ostream &os, EntityAction action);

#endif // ENTITY_ACTIONS_HPP_
