/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Keys.hpp file
*/

#ifndef KEYS_HPP
    #define KEYS_HPP

    #include <unordered_map>
    #include <string>

enum class Key : int {
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    UP_KEY, DOWN_KEY, LEFT_KEY, RIGHT_KEY,
    SPACE, ENTER, ESCAPE, DELETE, BACKSPACE,
    NUM_0, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,
    LEFT_CLICK, RIGHT_CLICK, MIDDLE_CLICK,
    UNKNOWN
};

extern const std::unordered_map<std::string, Key> strToKey;

/**
 * @brief Converts a string to a Key enum value.
 * 
 * @param keyString The string to convert to a Key enum value.
 * 
 * @return The Key enum value.
 */
Key stringToKey(const std::string &keyString);

std::ostream &operator<<(std::ostream &os, Key key);

#endif // KEYS_HPP
