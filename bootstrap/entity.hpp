/*
** EPITECH PROJECT, 2024
** RType
** File description:
** entity.hpp file
*/

#ifndef ENTITY_HPP
    #define ENTITY_HPP

#include <iostream>

class entity {
    public:
        explicit entity(size_t id) : _id(id) {};
        ~entity() = default;

        operator size_t() const {
            return _id;
        }
    protected:
        size_t _id;
    private:
};

#endif // ENTITY_HPP
