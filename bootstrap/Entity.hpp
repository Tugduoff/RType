/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Entity.hpp file
*/

#ifndef ENTITY_HPP
    #define ENTITY_HPP

    #include <cstddef>

class Entity {
    public:

        explicit Entity(size_t id) : __id(id) {};
        ~Entity() = default;

        operator size_t() const { return __id; }

    private:

        size_t __id;
};

#endif // ENTITY_HPP
