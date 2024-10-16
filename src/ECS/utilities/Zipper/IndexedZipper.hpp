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
using IndexedZipper = GenericZipper<IndexedZipperIterator<Containers...>, Containers...>;

#endif /* INDEXED_ZIPPER_HPP */
