/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ISystem.hpp file
*/

#ifndef ISYSTEM_HPP
    #define ISYSTEM_HPP

namespace Engine {
    class GameEngine;
}

/**
 * @namespace Systems
 * @brief Namespace for all ECS Systems.
 */
namespace Systems {
    /**
     * @class ISystem
     * 
     * @brief Interface for all our Systems.
     * 
     * This class is an interface for all ECS systems. It provides methods to run the system and initialize it.
     */
    class ISystem {
        public:
            virtual ~ISystem() = default;

            /**
             * @brief Function representing the system that will be called by the ECS
             * 
             * @param reg : The registry of the ECS
             * @note This function should contain the logic of the system
             */
            virtual void run(Engine::GameEngine &engine) = 0;

            /**
             * @brief Function called when the system is initialized
             * 
             * @param engine : The game engine
             * 
             * @return true : If the system is initialized, false if an error happened
             * 
             * @throw std::runtime_error : an error happened
             * 
             * @note This function should be used to initialize the system and must be called at the start of the game
             */
            virtual void init(Engine::GameEngine &engine) = 0;

        private:
    };
};

#endif // ISYSTEM_HPP
