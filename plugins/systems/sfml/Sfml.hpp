/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Sfml.hpp file
*/

#ifndef SFML_HPP
    #define SFML_HPP

    #include <SFML/Graphics.hpp>
    #include <libconfig.h++>
    #include "systems/ASystem.hpp"
    #include "Display.hpp"
    #include "Input.hpp"
    #include "Chrono.hpp"

namespace Systems {
    class Sfml : public ASystem {
        public:

            Sfml();
            Sfml(libconfig::Setting &config);
            ~Sfml() = default;

            void run(Engine::GameEngine &engine) override;
            void init(Engine::GameEngine &engine) override;

            Display displaySystem;
            Input inputSystem;

        private:

            sf::RenderWindow __window;
            Chrono __chrono;
    };
};

#endif // SFML_HPP
