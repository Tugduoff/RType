/*
** EPITECH PROJECT, 2024
** RType
** File description:
** MoveSystem.hpp file
*/

#ifndef MOVESYSTEM_HPP
    #define MOVESYSTEM_HPP

    #include <libconfig.h++>
    #include "systems/ASystem.hpp"

namespace Engine {
    class GameEngine;
}

namespace Systems {
    /**
     * @class MoveSystem
     * 
     * @brief System class from Systems that moves entities.
     * 
     * This system is responsible for moving entities based on their velocity and position components.
     */
    class MoveSystem : public ASystem {
        public:
            MoveSystem() = default;
            MoveSystem(libconfig::Setting &config);
            ~MoveSystem() = default;

            void run(Engine::GameEngine &engine) override;
            void init(Engine::GameEngine &engine) override;
        private:
    };
};

#endif // MOVESYSTEM_HPP
