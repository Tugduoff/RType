/*
** EPITECH PROJECT, 2024
** BsRtype
** File description:
** Registry.hpp file
*/

#ifndef REGISTRY_HPP
    #define REGISTRY_HPP

    #include "ECS/component/ComponentManager.hpp"
    #include "ECS/entity/EntityManager.hpp"
    #include "ECS/system/SystemManager.hpp"

namespace ECS {
    class Registry {
        public:

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

        private:

            EntityManager __entityManager;
            ComponentManager __componentManager;
            SystemManager __systemManager;

    };
};

#endif // REGISTRY_HPP
