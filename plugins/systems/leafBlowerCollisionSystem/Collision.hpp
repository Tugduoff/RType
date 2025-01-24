/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Collision.hpp file
*/

#ifndef COLLISION_LFB_HPP
    #define COLLISION_LFB_HPP

    #include <libconfig.h++>
    #include "systems/ASystem.hpp"

namespace Systems {
    /**
     * @class Collision
     * 
     * @brief System class from Systems that handles the Raylib window and input.
     * 
     * This system is responsible for creating the Raylib window and handling input events.
     * Thus, it is responsible for running the Collision and Input systems.
     */
    class Collision : public ASystem {
        public:

            Collision();
            Collision(libconfig::Setting &config);
            ~Collision() = default;

            void run(Engine::GameEngine &engine) override;
            void init(Engine::GameEngine &engine) override;

        private:

            uint32_t __playerForce;

    };
};

#endif // COLLISION_LFB_HPP
