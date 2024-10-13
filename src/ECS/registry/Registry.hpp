/*
** EPITECH PROJECT, 2024
** Rtype
** File description:
** Registry.hpp file
*/

#ifndef REGISTRY_HPP
    #define REGISTRY_HPP

    #include "ECS/component/ComponentManager.hpp"
    #include "ECS/entity/Entity.hpp"
    #include "ECS/entity/EntityManager.hpp"
    #include "ECS/system/SystemManager.hpp"
    #include <functional>
    #include <vector>

/**
 * @namespace ECS
 * 
 * @brief Main namespace for the ECS
 */
namespace ECS {
    /**
     * @class Registry
     * 
     * @brief holds the component, entity and system managers of the ECS
     * 
     * This class is a singleton that holds the component, entity and system managers.
     */
    class Registry {
        public:

            /**
             * @brief Member type used to denote callbacks
             * @brief called when an entity is created.
             */
            using EntityCreateCallback = std::function<void(const Entity &e)>;

            /**
             * @brief Member type used to denote callbacks
             * @brief called when an entity is killed.
             */
            using EntityKillCallback = std::function<void(const Entity &e)>;

            /**
             * @brief Get the component manager
             * 
             * @return ComponentManager& : the component manager
             */
            ComponentManager &componentManager() { return __componentManager; };

            /**
             * @brief Get the entity manager
             * 
             * @return EntityManager& : the entity manager
             */
            EntityManager &entityManager() { return __entityManager; };

            /**
             * @brief Get the system manager
             * 
             * @return SystemManager& : the system manager
             */
            SystemManager &systemManager() { return __systemManager; };

            /**
             * @brief Kill an entity and destroy all attached components,
             * @brief and also call all registered callbacks for that event
             *
             * @param entity The entity to kill
             */
            void killEntity(const Entity &entity)
            {
                this->__componentManager.removeAllFromEntity(entity);
                this->__entityManager.killEntity(entity);
                for (auto const &cb : this->__killCallbacks) {
                    cb(entity);
                }
            }

            /**
             * @brief Create an entity and notify all registered
             * @brief callbacks for that event
             */
            Entity createEntity()
            {
                Entity entity = this->__entityManager.spawnEntity();

                for (auto const &cb : this->__createCallbacks) {
                    cb(entity);
                }
                return entity;
            }

            /**
             * @brief Register a callback that will be called everytime an
             * @brief entity is created by createEntity
             */
            void addEntityCreateCallback(EntityCreateCallback cb)
            {
                __createCallbacks.push_back(std::move(cb));
            }

            /**
             * @brief Register a callback that will be called everytime an
             * @brief entity is killed by killEntity
             */
            void addEntityKillCallback(EntityKillCallback cb)
            {
                __killCallbacks.push_back(std::move(cb));
            }

        private:

            EntityManager __entityManager;
            ComponentManager __componentManager;
            SystemManager __systemManager;

            std::vector<EntityCreateCallback> __createCallbacks;
            std::vector<EntityKillCallback> __killCallbacks;

    };
};

#endif // REGISTRY_HPP
