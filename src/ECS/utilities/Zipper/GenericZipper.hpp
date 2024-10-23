/*
** EPITECH PROJECT, 2024
** RType
** File description:
** GenericZipper.hpp file
*/

#ifndef GENERIC_ZIPPER_HPP
    #define GENERIC_ZIPPER_HPP

    // #if __cplusplus < 202002L
    //     #error "Generic Zipper requires C++20 features, which are not provided by current c++ version"
    // #endif

    #include <algorithm>
    #include <cstddef>
#include <utility>
    #include <vector>

template<typename T>
constexpr inline T vmin(T &&t)
{
    return std::forward<T>(t);
}

template<typename T, typename... Args>
constexpr inline T vmin(T v1, T v2, Args... vals)
{
    if (v1 < v2) {
        return vmin(v1, std::forward<Args>(vals)...);
    } else {
        return vmin(v2, std::forward<Args>(vals)...);
    }
}

template<typename T>
T vmax(T &&t)
{
    return std::forward<T>(t);
}

template<typename T>
T vmax(T v1, T v2, T vals...)
{
    if (v1 > v2) {
        return vmax(v1, std::forward(vals));
    } else {
        return vmax(v2, std::forward(vals));
    }
}

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
