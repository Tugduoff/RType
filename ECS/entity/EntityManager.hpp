/*
** EPITECH PROJECT, 2024
** RType
** File description:
** EntityManager.hpp file
*/

#ifndef ENTITY_MANAGER_HPP
    #define ENTITY_MANAGER_HPP

    #include "Entity.hpp"
    #include <vector>

namespace ECS {
    /**
     * @class EntityManager
     * 
     * @brief Class that manages entities inside the ECS
     */
    class EntityManager {
        public:

            /**
             * @brief Spawn a new entity
             */
            Entity spawnEntity();

            /**
             * @brief Get an entity from its index
             * 
             * @param idx : the index
             * 
             * @return Entity
             */
            Entity entityFromIndex(size_t idx);

            /**
             * @brief Kill an entity
             * 
             * @param e : the entity
             */
            void killEntity(Entity const &e);

        private:

            std::vector<Entity> __entities;
            size_t __nextEntityId = 0;
    };
};

#endif // ENTITY_MANAGER_HPP
