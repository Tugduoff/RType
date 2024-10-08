/*
** EPITECH PROJECT, 2024
** RType
** File description:
** SystemManager.hpp file
*/

#ifndef SYSTEM_MANAGER_HPP
    #define SYSTEM_MANAGER_HPP

    #include "plugins/systems/ISystem.hpp"
    #include <vector>
    #include <memory>

namespace Engine {
    class GameEngine;
}

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
            void addSystem(std::unique_ptr<Systems::ISystem> system);

            /**
             * @brief Run all the systems
             */
            void run(Engine::GameEngine &engine);

            /**
             * @brief Run all systems init function
             * 
             * @note This function will run all systems init function.
             */
            void initSystems(Engine::GameEngine &engine);

        private:

            std::vector<std::unique_ptr<Systems::ISystem>> __systems;
    };
}

#endif // SYSTEM_MANAGER_HPP
