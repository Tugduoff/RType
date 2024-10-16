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

template<class... Containers>
using Zipper = GenericZipper<ZipperIterator<Containers...>, Containers...>;

#endif /* ZIPPER_HPP */
