/*
** EPITECH PROJECT, 2024
** RType
** File description:
** IndexedZipper.hpp file
*/

#ifndef INDEXED_ZIPPER_HPP
    #define INDEXED_ZIPPER_HPP

    #include "IndexedZipperIterator.hpp"
    #include "GenericZipper.hpp"

/**
 * @class IndexedZipper
 *
 * @brief A helper class to iterate over several containers
 * @brief at the same time, returning only valid tuples
 * @brief of values, as well as the index of these tuples
 *
 * @tparam Containers The container types to iterate over (deduced automatically)
 *
 * @see Zipper which provides similar functionality
 *
 * @see IndexedZipperIterator, the iterator type of IndexedZipper
 */
template<class... Containers>
class IndexedZipper : public GenericZipper<
    IndexedZipperIterator<Containers...>, 
    Containers...
> {
public:
    /**
     * @brief Constructor for an IndexedZipper
     *
     * @param cs The containers to zip
     */
    IndexedZipper(Containers &...cs) : GenericZipper<
        IndexedZipperIterator<Containers...>, 
        Containers...
    >(cs...) {}
};

#endif /* INDEXED_ZIPPER_HPP */
