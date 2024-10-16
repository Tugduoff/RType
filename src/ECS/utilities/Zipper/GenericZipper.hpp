/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Zipper.hpp file
*/

#ifndef GENERIC_ZIPPER_HPP
    #define GENERIC_ZIPPER_HPP

    #include <algorithm>
    #include <cstddef>
    #include <vector>

template<class ZipIt, class... Containers>
class GenericZipper {
public:

    using iterator = ZipIt;
    using iterator_tuple = typename iterator::iterator_tuple;

    GenericZipper(Containers &...cs)
    :   _begin(_compute_begin(cs...)),
        _end(_compute_end(cs...)),
        _size(_compute_size(cs...))
    {
    }

    iterator begin()
    {
        return {_begin, _size};
    }

    iterator end()
    {
        return {_end, _size};
    }

private:

    static size_t _compute_size(Containers &... containers)
    {
        std::vector<std::size_t> sizes = {containers.size()...};

        return std::ranges::min(sizes);
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
