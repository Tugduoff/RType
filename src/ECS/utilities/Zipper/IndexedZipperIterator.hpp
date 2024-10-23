/*
** EPITECH PROJECT, 2024
** RType
** File description:
** IndexedZipperIterator.hpp file
*/

#ifndef INDEXED_ZIPPER_ITERATOR_HPP
    #define INDEXED_ZIPPER_ITERATOR_HPP

    #include "ZipperIterator.hpp"
    #include "ECS/utilities/TupleUtils.hpp"
    #include <cstddef>
    #include <tuple>
    #include <utility>

template<class ZipIt, class... Containers>
class GenericZipper;

/**
 * @class IndexedZipperIterator
 *
 * @brief Iterator helper class for the IndexedZipper pseudo-container
 * @brief Iterates over several containers, returning valid tuples and their index
 *
 * @tparam Containers The container types to iterate over
 *
 * @see ZipperIterator the non-indexed zipper which implement the zip logic
 *
 * @see IndexedZipper the pseudo-container which uses this iterator
 */
template<class... Containers>
class IndexedZipperIterator : public ZipperIterator<Containers...> {
    using parent = ZipperIterator<Containers...>;

public:

    /**
     * @brief Type returned by the iterator's operator*
     */
    using value_type = prependTupleType<typename parent::value_type, std::size_t>;

    using reference = value_type;
    using pointer = parent::pointer;
    using difference_type = parent::difference_type;
    using iterator_category = parent::iterator_category;
    using iterator_tuple = parent::iterator_tuple;

    // This allows only GenericZipper to construct an IndexedZipperIterator
    friend GenericZipper<IndexedZipperIterator, Containers...>;

    /**
     * @brief Constructor for an IndexedZipperIterator
     *
     * @param it_tuple The iterators to zip
     * @param max The maximum length of passed iterators
     */
    IndexedZipperIterator(iterator_tuple const &it_tuple, size_t max)
    :   parent(it_tuple, max)
    {
    }

public:

    /**
     * @brief Copy constructor for an IndexedZipper
     */
    IndexedZipperIterator(IndexedZipperIterator const &other)
    :   parent(other)
    {
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

protected:
    template<size_t... Is>
    value_type to_value(std::index_sequence<Is...>)
    {
        return std::tuple_cat(std::make_tuple(parent::_idx), parent::to_value(_seq));
    }

private:
    static constexpr std::index_sequence_for<Containers...> _seq {};
};

#endif /* INDEXED_ZIPPER_ITERATOR_HPP */
