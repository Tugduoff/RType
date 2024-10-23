/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ZipperIterator.hpp file
*/

#ifndef ZIPPER_ITERATOR_HPP
    #define ZIPPER_ITERATOR_HPP

    #if __cplusplus < 202002L
        #error "ZipperIterator requires C++20 features, which are not provided by current c++ version"
    #endif

    #include <cstddef>
    #include <iterator>
    #include <tuple>
    #include <utility>

template<class ZipIt, class... Containers>
class GenericZipper;

/**
 * @class ZipperIterator
 *
 * @brief Helper class to iterate over valid tuples of
 * @brief values in multiple containers
 *
 * @tparam Containers the types of zipped containers
 *
 * @see Zipper, the pseudo-container which uses ZipperIterator
 *
 * @see IndexedZipperIterator, providing similar functionality
 */
template<class... Containers>
class ZipperIterator {
protected:
    template<class Container>
    using iterator_t = decltype(std::declval<Container>().begin());

    template<class Container>
    using it_reference_t = typename iterator_t<Container>::reference;

public:
    /**
     * @brief Type returned by the iterator's operator*
     */
    using value_type = std::tuple<decltype(*std::declval<typename iterator_t<Containers>::value_type>()) &...>;

    using reference = value_type;
    using pointer = void;
    using difference_type = size_t;
    using iterator_category = std::forward_iterator_tag;
    using iterator_tuple = std::tuple<iterator_t<Containers>...>;

    // This allows only GenericZipper to use private members of ZipperIterator
    friend GenericZipper<ZipperIterator, Containers...>;

    /**
     * @brief Contructor for a ZipperIterator
     *
     * @param it_tuple A tuple of iterators to zip
     * @param max The maximum index to use in zipped iterators
     *
     * @note This constructor is private, so it can only be called
     * @note by GenericZipper, because it is a friend
     */
    ZipperIterator(iterator_tuple const &it_tuple, size_t max)
    :   _current(it_tuple),
        _max(max),
        _idx(0)
    {
        if (_idx < max && !all_set(_seq)) {
            incr_all(_seq);
        }
    }

public:
    /**
     * @brief Copy contructor for a ZipperIterator
     */
    ZipperIterator(ZipperIterator const &other)
    :   _current(other._current),
        _max(other._max),
        _idx(other._idx)
    {
    }

    /**
     * @brief Prefix increment operator used to increment the zipper
     * @brief to the next valid tuple of values
     *
     * @returns A reference to this iterator
     */
    ZipperIterator &operator++()
    {
        incr_all(_seq);
        return *this;
    }

    /**
     * @brief Postfix increment operator used to increment the zipper
     * @brief to the next valid tuple of values
     *
     * @returns A copy of this iterator, before the increment has been done
     */
    ZipperIterator operator++(int)
    {
        auto ret = *this;

        incr_all(_seq);
        return ret;
    }

    /**
     * @brief Dereference operator
     *
     * @returns A tuple of references to zipped values
     */
    value_type operator*()
    {
        return this->to_value(_seq);
    }

    /**
     * @brief Dereference member-access operator
     *
     * @returns A tuple of references to zipped values
     */
    value_type operator->()
    {
        return this->to_value(_seq);
    }

    /**
     * @brief Equality operator
     *
     * @returns whether the two compared iterators contain the same references
     */
    friend bool operator==(ZipperIterator const &lhs, ZipperIterator const &rhs)
    {
        return lhs._current == rhs._current;
    }

    /**
     * @brief Inequality operator
     *
     * @returns whether the two compared iterators contain different references
     */
    friend bool operator!=(ZipperIterator const &lhs, ZipperIterator const &rhs)
    {
        return !(lhs == rhs);
    }

    

private:
    // Increment every iterator at the same time. It also skips to the next
    // value if one of the pointed to std::optional does not contains a
    // value.
    template<std::size_t... Is>
    void incr_all(std::index_sequence<Is...>)
    {
        do {
            std::make_tuple(++(std::get<Is>(_current))...);
            _idx++;
        } while (_idx < _max && !this->all_set(_seq));
    }

    // check if every std::optional are set .
    template<std::size_t... Is>
    bool all_set(std::index_sequence<Is...>)
    {
        return ((bool)*std::get<Is>(_current) && ...);
    }

protected:

    // return a tuple of reference to components .
    template<size_t... Is>
    value_type to_value(std::index_sequence<Is...>)
    {
        return std::tie(**std::get<Is>(_current)...);
    }

protected:
    iterator_tuple _current;
    size_t _max; // compare this value to _idx to prevent infinite loop .
    size_t _idx;
    static constexpr std::index_sequence_for<Containers...> _seq {};
};

#endif /* ZIPPER_ITERATOR_HPP */
