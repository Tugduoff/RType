/*
** EPITECH PROJECT, 2024
** RType
** File description:
** SystemManager.hpp file
*/

#ifndef SYSTEM_MANAGER_HPP
    #define SYSTEM_MANAGER_HPP

    #include <vector>

namespace Systems {
    class ISystem;
};

namespace ECS {
    class Registry;

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
            void addSystem(Systems::ISystem *system);

            /**
             * @brief Add a system to the manager
             * 
             * @param system : the system
             */
            void addSystem(const Systems::ISystem *system);

            /**
             * @brief Run all the systems
             */
            void run(Registry &reg);

        private:

            std::vector<Systems::ISystem *> __systems;
    };
}

#endif // SYSTEM_MANAGER_HPP
