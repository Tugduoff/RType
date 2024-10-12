/*
** EPITECH PROJECT, 2024
** Rtype
** File description:
** ValueIterator.hpp file
*/

#ifndef VALUE_ITERATOR_HPP
    #define VALUE_ITERATOR_HPP

    #include <utility>

/**
 * @brief Helper class for iterating over the values of a map
 *
 * @tparam The iterator type, for example: `std::map<int, int>::iterator`
 *
 * Inspired by https://stackoverflow.com/a/23608376
 */
template<typename It>
struct ValueIterator : public It {
    ValueIterator(It src) : It(std::move(src)) {}

    auto &operator*() const
    {
        return It::operator*().second;
    }
};

#endif /* VALUE_ITERATOR_HPP */
