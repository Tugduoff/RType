/*
** EPITECH PROJECT, 2024
** RType
** File description:
** SystemManager.hpp file
*/

#ifndef SYSTEM_MANAGER_HPP
    #define SYSTEM_MANAGER_HPP

    #include "ISystem.hpp"
    #include <vector>

namespace ECS {
    /**
     * @class SystemManager
     * 
     * @brief Class that manages systems inside the ECS
     */
    class SystemManager {
        public:

            /**
             * @brief Add a system to the manager
             * 
             * @param system : the system
             */
            void addSystem(ISystem *system);

            /**
             * @brief Add a system to the manager
             * 
             * @param system : the system
             */
            void addSystem(const ISystem *system);

            /**
             * @brief Run all the systems
             */
            void run(Registry &reg);

        private:

            std::vector<ISystem *> __systems;
    };
}

#endif // SYSTEM_MANAGER_HPP
