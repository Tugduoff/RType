/*
** EPITECH PROJECT, 2024
** RType
** File description:
** TupleUtils.hpp file
*/

#ifndef TUPLE_UTILS_HPP
    #define TUPLE_UTILS_HPP

    #include <tuple>

/**
 * @brief Helper type used to declare tuple types that
 * @brief result from concatenating two or more tuples
 *
 * @tparam tups The tuple types to concatenate
 */
template <typename... tups>
using NTupleCombination = decltype(std::tuple_cat(std::declval<tups>()...));

/**
 * @brief Prepends zero or more types to the start
 * @brief of the type list of a tuple type
 *
 * @tparam Tup The tuple type to prepend types to
 * @tparam types The types to prepend to the tuple
 */
template <typename Tup, typename... types>
using prependTupleType = NTupleCombination<
    decltype(std::make_tuple(std::declval<types>()...)),
    decltype(std::declval<Tup>())
>;

/**
 * @brief Appends zero or more types to the end
 * @brief of the type list of a tuple type
 *
 * @tparam Tup The tuple type to append types to
 * @tparam types The types to append to the tuple
 */
template <typename Tup, typename... types>
using appendTupleType = NTupleCombination<
    decltype(std::declval<Tup>()),
    decltype(std::make_tuple(std::declval<types>()...))
>;

#endif /* TUPLE_UTILS */
