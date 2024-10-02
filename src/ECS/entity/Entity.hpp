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

            /**
             * @brief Constructor for an entity
             *
             * @param id The unique identifier of this entity
             */
            explicit Entity(size_t id) : __id(id) {};

            /**
             * @brief Copy constructor for an Entity
             */
            Entity(const Entity &other) = default;

            /**
             * @brief Move constructor for an entity
             */
            Entity(Entity &&other) = default;


            /**
             * @brief Assignment operator for an entity
             */
            Entity &operator=(const Entity &other) = default;

            /**
             * @brief Move assignment operator for an entity
             */
            Entity &operator=(Entity &&other) = default;

            ~Entity() = default;

            /**
             * @brief operator size_t() that returns the size_t of the entity
             * 
             * @return size_t : The size of the entity
             */
            operator size_t() const { return __id; }

        private:

            size_t __id;
    };
};

#endif // ENTITY_HPP
