/*
** EPITECH PROJECT, 2024
** RType
** File description:
** GenericZipper.hpp file
*/

#ifndef GENERIC_ZIPPER_HPP
    #define GENERIC_ZIPPER_HPP

    #include <cstddef>
    #include <tuple>
    #include "utils/MinMax.hpp"

/**
 * @class Generic zipper
 *
 * @brief Helper class that is used to implement Zipper and
 * @brief IndexedZipper from their respective iterator classes
 *
 * @tparam ZipIt The ZipperIterator to use
 * @tparam Containers the types of the zipped containers (deduced automatically)
 */
template<class ZipIt, class... Containers>
class GenericZipper {
public:

    using iterator = ZipIt;
    using iterator_tuple = typename iterator::iterator_tuple;

    /**
     * @brief GenericZipper constructor
     *
     * @param cs The containers to zip
     */
    GenericZipper(Containers &...cs)
    :   _begin(_compute_begin(cs...)),
        _end(_compute_end(cs...)),
        _size(_compute_size(cs...))
    {
    }

    /**
     * @brief begin method which used to obtain a begin iterator;
     * @brief called automatically by the ranged-base `for` syntax
     *
     * @returns An iterator containing the first valid zipped tuple
     *
     * @note If the zipped containers contain no valid tuple,
     * @note begin() and end() will return the same iterator
     */
    iterator begin()
    {
        return {_begin, _size};
    }

    /**
     * @brief end method which used to obtain an end iterator;
     * @brief called automatically by the range-based `for` syntax
     *
     * @returns The first out-of-range iterator
     *
     * @note If the zipped containers contain no valid tuple,
     * @note begin() and end() will return the same iterator
     */
    iterator end()
    {
        return {_end, 0};
    }

private:

    static size_t _compute_size(Containers &... containers)
    {
        return vmin(containers.size()...);
    }

    static iterator_tuple _compute_begin(Containers &... containers)
    {
        return std::make_tuple(containers.begin()...);
    }

    static iterator_tuple _compute_end(Containers &... containers)
    {
        std::size_t size = _compute_size(containers...);

        return std::make_tuple((containers.begin() + size)...);
    }

private:
    iterator_tuple _begin;
    iterator_tuple _end;
    std::size_t _size;
};


#endif /* GENERIC_ZIPPER_HPP */
