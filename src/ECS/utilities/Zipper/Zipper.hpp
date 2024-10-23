/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Zipper.hpp file
*/

#ifndef ZIPPER_HPP
    #define ZIPPER_HPP

    #include "ZipperIterator.hpp"
    #include "GenericZipper.hpp"

/**
 * @class Zipper
 *
 * @brief A pseudo-container used to iterate over several containers
 * @brief at the same time, returning only valid tuples of values
 *
 * @tparam Containers The container types to iterate over (deduced automatically)
 *
 * @see IndexedZipper which provides similar functionality
 *
 * @see ZipperIterator the iterator class used by Zipper
 */
template<class... Containers>
class Zipper : public GenericZipper<
    ZipperIterator<Containers...>, 
    Containers...
> {
public:
    /**
     * @brief Constructor for an Zipper
     *
     * @param cs The containers to zip
     */
    Zipper(Containers &...cs) : GenericZipper<
        ZipperIterator<Containers...>, 
        Containers...
    >(cs...) {}
};

#endif /* ZIPPER_HPP */
