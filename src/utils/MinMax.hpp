/*
** EPITECH PROJECT, 2024
** RType
** File description:
** MinMax.hpp file
*/

#ifndef MIN_MAX_HPP
    #define MIN_MAX_HPP

    #include <utility>

/**
 * @brief Template specialisation of the below vmin function
 * @brief used as the base in case for recursion
 */
template<typename T>
constexpr inline T vmin(T &&t)
{
    return std::forward<T>(t);
}

/**
 * @brief Function to compute the smallest value from a parameter pack
 *
 * @tparam T The type to compare
 * @tparam Args the types of following arguments
 *
 * @param v1 The first value to compare
 * @param v2 The second value to compare
 * @param vals Parameter pack of as many following items to compare
 *
 * @returns The smallest value from all given parameters
 *
 * @code assert(vmin(45, 12, 7, 541) == 7)
 */
template<typename T, typename... Args>
constexpr inline T vmin(T v1, T v2, Args... vals)
{
    if (v1 < v2) {
        return vmin(v1, std::forward<Args>(vals)...);
    } else {
        return vmin(v2, std::forward<Args>(vals)...);
    }
}

/**
 * @brief Template specialisation of the below vmax function
 * @brief used as the base in case for recursion
 */
template<typename T>
constexpr inline T vmax(T &&t)
{
    return std::forward<T>(t);
}

/**
 * @brief Function to compute the greatest value from a parameter pack
 *
 * @tparam T The type to compare
 * @tparam Args the types of following arguments
 *
 * @param v1 The first value to compare
 * @param v2 The second value to compare
 * @param vals Parameter pack of as many following items to compare
 *
 * @returns The greatest value from all given parameters
 *
 * @code assert(vmax(45, 12, 7, 541) == 541)
 */
template<typename T, typename... Args>
constexpr inline T vmax(T v1, T v2, Args... vals)
{
    if (v1 > v2) {
        return vmax(v1, std::forward<Args>(vals)...);
    } else {
        return vmax(v2, std::forward<Args>(vals)...);
    }
}

#endif // MIN_MAX_HPP