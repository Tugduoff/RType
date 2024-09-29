/*
** EPITECH PROJECT, 2024
** RType
** File description:
** SystemManager.hpp file
*/

#ifndef SYSTEM_MANAGER_HPP
    #define SYSTEM_MANAGER_HPP

    #include <vector>
    #include <memory>
    #include "plugins/systems/ISystem.hpp"
    #include <iostream>

namespace ECS {
    class Registry;

    /**
     * @class SystemManager
     * 
     * @brief Class that manages systems inside the ECS
     */
    class SystemManager {
        public:

            ~SystemManager() {
                std::cout << "SystemManager destructor called" << std::endl;
                __systems.clear(); // Clears the vector, invoking the destructors of each unique_ptr
                std::cout << "Systems cleared" << std::endl;
            }

            /**
             * @brief Add a system to the manager
             * 
             * @param system : the system
             */
            void addSystem(std::unique_ptr<Systems::ISystem> system);

            /**
             * @brief Run all the systems
             */
            void run(Registry &reg);

        private:

            std::vector<std::unique_ptr<Systems::ISystem>> __systems;
    };
}

#endif // SYSTEM_MANAGER_HPP
