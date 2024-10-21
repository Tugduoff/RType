/*
** EPITECH PROJECT, 2024
** RType
** File description:
** TupleUtils.hpp file
*/

#ifndef TUPLE_UTILS_HPP
    #define TUPLE_UTILS_HPP

    #include <tuple>

template <typename... tups>
using NtupleCombination = decltype(std::tuple_cat(std::declval<tups>()...));

template <typename Tup, typename... types>
using prependTupleType = NtupleCombination<
    decltype(std::make_tuple(std::declval<types>()...)),
    decltype(std::declval<Tup>())
>;

template <typename Tup, typename... types>
using appendTupleType = NtupleCombination<
    decltype(std::declval<Tup>()),
    decltype(std::make_tuple(std::declval<types>()...))
>;

#endif /* TUPLE_UTILS */
