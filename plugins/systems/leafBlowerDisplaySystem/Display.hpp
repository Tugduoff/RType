/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Display.hpp file
*/

#ifndef DISPLAY_LFB_HPP
    #define DISPLAY_LFB_HPP

    #include <libconfig.h++>
    #include "systems/ASystem.hpp"
    #include "raylib.h"

namespace Systems {
    /**
     * @class Display
     * 
     * @brief System class from Systems that handles the Raylib window and input.
     * 
     * This system is responsible for creating the Raylib window and handling input events.
     * Thus, it is responsible for running the Display and Input systems.
     */
    class Display : public ASystem {
        public:

            Display();
            Display(libconfig::Setting &config);
            ~Display() = default;

            void run(Engine::GameEngine &engine) override;
            void init(Engine::GameEngine &engine) override;

        private:

            Texture2D __playerTexture;
            Texture2D __leafTexture;
    };
};

#endif // DISPLAY_LFB_HPP
