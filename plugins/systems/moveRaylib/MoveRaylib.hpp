/*
** EPITECH PROJECT, 2024
** RType
** File description:
** MoveRaylib.hpp file
*/

#ifndef MOVE_RAYLIB_HPP
    #define MOVE_RAYLIB_HPP

    #include <libconfig.h++>
    #include "systems/ASystem.hpp"

namespace Engine {
    class GameEngine;
}

namespace Systems {
    /**
     * @class MoveRaylib
     * 
     * @brief System class from Systems that moves entities.
     * 
     * This system is responsible for moving entities based on their velocity and position components.
     */
    class MoveRaylib : public ASystem {
        public:
            MoveRaylib() = default;
            MoveRaylib(libconfig::Setting &config);
            ~MoveRaylib() = default;

            void run(Engine::GameEngine &engine) override;
            void init(Engine::GameEngine &engine) override;
        private:
    };
};

#endif // MOVE_RAYLIB_HPP
