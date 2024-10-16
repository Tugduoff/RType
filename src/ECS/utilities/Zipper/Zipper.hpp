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
class Zipper : public GenericZipper<
    ZipperIterator<Containers...>, 
    Containers...
> {
public:
    Zipper(Containers &...cs) : GenericZipper<
        ZipperIterator<Containers...>, 
        Containers...
    >(cs...) {}
};

#endif /* ZIPPER_HPP */
