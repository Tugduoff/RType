/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ZipperIterator.hpp file
*/

#ifndef ZIPPER_ITERATOR_HPP
    #define ZIPPER_ITERATOR_HPP

    #include <cstddef>
    #include <iterator>
    #include <tuple>
    #include <utility>

template<class... Containers>
class Zipper;

template<class... Containers>
class ZipperIterator {
    template<class Container>
    using iterator_t = decltype(std::declval<Container>().begin());

    template<class Container>
    using it_reference_t = typename iterator_t<Container>::reference;

public:
    using value_type = std::tuple<it_reference_t<Containers>&...>;
    using reference = value_type;
    using pointer = void;
    using difference_type = size_t;
    using iterator_category = std::forward_iterator_tag;
    using iterator_tuple = std::tuple<iterator_t<Containers>...>;

    // If we want zipper_iterator to be built by zipper only.
    friend Zipper<Containers...>;

    ZipperIterator(iterator_tuple const &it_tuple, size_t max)
    :   _current(it_tuple),
        _max(max),
        _idx(0)
    {
    }

public:
    ZipperIterator(ZipperIterator const &other)
    :   _current(other._current),
        _max(other._max),
        _idx(other._idx)
    {
    }

    ZipperIterator &operator++()
    {
        incr_all();
        return *this;
    }

    ZipperIterator operator++(int)
    {
        auto ret = *this;

        incr_all();
        return ret;
    }

    value_type operator*()
    {
        return this->to_value();
    }

    value_type operator->()
    {
        return this->to_value();
    }

    friend bool operator==(ZipperIterator const &lhs, ZipperIterator const &rhs)
    {
        return lhs._current == rhs._current;
    }

    friend bool operator!=(ZipperIterator const &lhs, ZipperIterator const &rhs)
    {
        return !(lhs == rhs);
    }

    

private:
    // Increment every iterator at the same time. It also skips to the next
    // value if one of the pointed to std::optional does not contains a
    // value.
    template<std::size_t... Is>
    void incr_all(std::index_sequence<Is...> = _seq)
    {
        for (; _idx < _max && !this->all_set(); _idx++) {
            void(++(std::get<Is>(_current))...);
        }
    }

    // check if every std::optional are set .
    template<std::size_t... Is>
    bool all_set(std::index_sequence<Is...> = _seq)
    {
        return ((... && (std::get<Is>(this->to_value()))));
    }

    // return a tuple of reference to components .
    template<size_t... Is>
    value_type to_value(std::index_sequence<Is...> = _seq)
    {
        return std::tie(*std::get<Is>(_current)...);
    }

private:
    iterator_tuple _current;
    size_t _max; // compare this value to _idx to prevent infinite loop .
    size_t _idx;
    static constexpr std::index_sequence_for<Containers...> _seq {};
};

#endif /* ZIPPER_ITERATOR_HPP */
