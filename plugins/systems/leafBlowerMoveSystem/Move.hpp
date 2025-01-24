/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Move.hpp file
*/

#ifndef MOVE_LFB_HPP
    #define MOVE_LFB_HPP

    #include <libconfig.h++>
    #include "systems/ASystem.hpp"
    #include "raylib.h"

namespace Systems {
    /**
     * @class Move
     * 
     * @brief System class from Systems that handles the Raylib window and input.
     * 
     * This system is responsible for creating the Raylib window and handling input events.
     * Thus, it is responsible for running the Move and Input systems.
     */
    class Move : public ASystem {
        public:

            Move();
            Move(libconfig::Setting &config);
            ~Move() = default;

            void run(Engine::GameEngine &engine) override;
            void init(Engine::GameEngine &engine) override;

        private:

            uint32_t __speed;
    };
};

#endif // MOVE_LFB_HPP
