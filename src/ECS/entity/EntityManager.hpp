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
    #include <algorithm>
    #include <stdexcept>

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
            Entity spawnEntity() {
                if (__deadEntities.size() > 0) {
                    Entity e = __deadEntities.back();
                    __deadEntities.pop_back();
                    return e;
                }
                __entities.push_back(Entity(__nextEntityId++));
                return __entities.back();
            }

            /**
             * @brief Get an entity from its index
             * 
             * @param idx : the index
             * 
             * @return Entity
             *
             * @throws if the index is out of range
             */
            Entity entityFromIndex(size_t idx) {
                if (idx >= __entities.size())
                    throw std::runtime_error("Entity index out of range");
                return __entities[idx];
            }

            /**
             * @brief Kill an entity
             * 
             * @param e : the entity
             */
            void killEntity(Entity const &e) {
                __deadEntities.push_back(e);
                __entities.erase(std::remove(__entities.begin(), __entities.end(), e), __entities.end());
            }

            auto const &viewEntities() const {
                return __entities;
            }

        private:

            std::vector<Entity> __entities;
            size_t __nextEntityId = 0;
            std::vector<Entity> __deadEntities;
    };
};

#endif // ENTITY_MANAGER_HPP
