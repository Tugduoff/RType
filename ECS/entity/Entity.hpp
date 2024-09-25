/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Entity.hpp file
*/

#ifndef ENTITY_HPP
    #define ENTITY_HPP

    #include <cstddef>

namespace ECS {
    /**
     * @class Entity
     * 
     * @brief Class that represents an entity in the ECS
     */
    class Entity {
        public:

            explicit Entity(size_t id) : __id(id) {};
            ~Entity() = default;

            /**
             * @brief operator () that returns the size_t of the entity
             * 
             * @return size_t : The size of the entity
             */
            operator size_t() const { return __id; }

        private:

            size_t __id;
    };
};

#endif // ENTITY_HPP
