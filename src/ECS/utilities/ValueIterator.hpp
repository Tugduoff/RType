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
 * @brief Helper class for iterating over the values of a container (e.g., a vector)
 *
 * @tparam The iterator type, for example: `std::vector<std::unique_ptr<Component>>::iterator`
 *
 */
template<typename It>
struct ValueIterator : public It {
    ValueIterator(It src) : It(std::move(src)) {}

    auto &operator*() const
    {
        return It::operator*();
    }
};

#endif /* VALUE_ITERATOR_HPP */
