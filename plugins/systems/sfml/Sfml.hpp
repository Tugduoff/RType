/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Sfml.hpp file
*/

#ifndef SFML_HPP
    #define SFML_HPP

    #include <SFML/Graphics.hpp>
    #include <SFML/Audio/Music.hpp>
    #include <libconfig.h++>
    #include "systems/ASystem.hpp"
    #include "Display.hpp"
    #include "Input.hpp"
    #include "ActionManager.hpp"
    #include "SoundSystem.hpp"

namespace Systems {
    /**
     * @class Sfml
     * 
     * @brief System class from Systems that handles the SFML window and input.
     * 
     * This system is responsible for creating the SFML window and handling input events.
     * Thus, it is responsible for running the Display and Input systems.
     */
    class Sfml : public ASystem {
        public:

            Sfml();
            Sfml(libconfig::Setting &config);
            ~Sfml() = default;

            void run(Engine::GameEngine &engine) override;
            void init(Engine::GameEngine &engine) override;

            Display displaySystem;
            Input inputSystem;
            ActionManager actionManager;
            SoundSystem soundSystem;

        private:

            sf::RenderWindow __window;

    };
};

#endif // SFML_HPP
