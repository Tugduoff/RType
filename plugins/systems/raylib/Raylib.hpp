/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Raylib.hpp file
*/

#ifndef RAYLIB_HPP
    #define RAYLIB_HPP

    #include <raylib.h>
    #include <libconfig.h++>
    #include "systems/ASystem.hpp"
    #include "DisplayManager.hpp"
    #include "InputManager.hpp"

namespace Systems {
    /**
     * @class Raylib
     * 
     * @brief System class from Systems that handles the Raylib window and input.
     * 
     * This system is responsible for creating the Raylib window and handling input events.
     * Thus, it is responsible for running the Display and Input systems.
     */
    class Raylib : public ASystem {
        public:

            Raylib();
            Raylib(libconfig::Setting &config);
            ~Raylib() = default;

            void run(Engine::GameEngine &engine) override;
            void init(Engine::GameEngine &engine) override;

            DisplayManager displayManager;
            InputManager inputManager;

        private:

    };
};

#endif // RAYLIB_HPP
