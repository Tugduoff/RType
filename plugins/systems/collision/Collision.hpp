/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Collision.hpp file
*/

#ifndef COLLISION_HPP
    #define COLLISION_HPP

    #include <libconfig.h++>
    #include "systems/ASystem.hpp"

namespace Engine {
    class GameEngine;
}

namespace Systems {
    /**
     * @class Collision
     * 
     * @brief System class from Systems that handles the collision.
     * 
     * This system is responsible for handling the collision between entities.
     */
    class Collision : public ASystem {
        public:
            Collision() = default;
            Collision(libconfig::Setting &config);
            ~Collision() = default;

            void run(Engine::GameEngine &engine) override;
            void init(Engine::GameEngine &engine) override;
        private:
    };
};

#endif // COLLISION_HPP
