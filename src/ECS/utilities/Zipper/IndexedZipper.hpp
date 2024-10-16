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

template<class... Containers>
class IndexedZipper : public GenericZipper<
    IndexedZipperIterator<Containers...>, 
    Containers...
> {
public:
    IndexedZipper(Containers &...cs) : GenericZipper<
        IndexedZipperIterator<Containers...>, 
        Containers...
    >(cs...) {}
};

#endif /* INDEXED_ZIPPER_HPP */
