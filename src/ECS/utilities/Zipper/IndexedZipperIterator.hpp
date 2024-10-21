/*
** EPITECH PROJECT, 2024
** RType
** File description:
** IndexedZipperIterator.hpp file
*/

#ifndef INDEXED_Zipper_ITERATOR_HPP
    #define INDEXED_Zipper_ITERATOR_HPP

    #include "ZipperIterator.hpp"
    #include <cstddef>
    #include <tuple>
    #include <utility>

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

template<class ZipIt, class... Containers>
class GenericZipper;

template<class... Containers>
class IndexedZipperIterator : public ZipperIterator<Containers...> {
    using this_t = IndexedZipperIterator<Containers...>;
    using parent = ZipperIterator<Containers...>;

    template<class Container>
    using iterator_t = parent::template iterator_t<Container>;

public:
    using value_type = prependTupleType<typename parent::value_type, std::size_t>;
    using reference = value_type;
    using pointer = parent::pointer;
    using difference_type = parent::difference_type;
    using iterator_category = parent::iterator_category;
    using iterator_tuple = parent::iterator_tuple;

    // If we want indexed_zipper_iterator to be built by indexed_zipper only.
    friend GenericZipper<IndexedZipperIterator, Containers...>;

    IndexedZipperIterator(iterator_tuple const &it_tuple, size_t max)
    :   parent(it_tuple, max)
    {
    }

public:
    IndexedZipperIterator(IndexedZipperIterator const &other)
    :   parent(other)
    {
    }

    value_type operator*()
    {
        return this->to_value(_seq);
    }

    value_type operator->()
    {
        return this->to_value(_seq);
    }

protected:
    template<size_t... Is>
    value_type to_value(std::index_sequence<Is...>)
    {
        return std::tuple_cat(std::make_tuple(parent::_idx), parent::to_value(_seq));
    }

private:
    static constexpr std::index_sequence_for<Containers...> _seq {};
};

#endif /* Indexed_Zipper_ITERATOR_HPP */
