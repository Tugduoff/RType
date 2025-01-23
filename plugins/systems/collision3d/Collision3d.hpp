/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Collision3d.hpp file
*/

#ifndef COLLISION_3D_HPP
    #define COLLISION_3D_HPP

    #include <libconfig.h++>
    #include "systems/ASystem.hpp"
    #include "ECS/entity/Entity.hpp"
    #include "utils/Chrono.hpp"

namespace Engine {
    class GameEngine;
}

namespace Systems {
    /**
     * @class Collision3d
     * 
     * @brief System class from Systems that handles the collision.
     * 
     * This system is responsible for handling the collision between entities.
     */
    class Collision3d : public ASystem {
        public:
            Collision3d() = default;
            Collision3d(libconfig::Setting &config);
            ~Collision3d() = default;

            void run(Engine::GameEngine &engine) override;
            void init(Engine::GameEngine &engine) override;
        private:

            /**
             * @brief Check the collision between player projectiles and enemies.
             * 
             * @param reg The registry of the game engine
             * 
             * @note This function will check the collision between player projectiles and enemies.
             */
            void checkPlayerToItemBoxCollision3d(Engine::GameEngine &engine);
    };
};

#endif // COLLISION_3D_HPP
