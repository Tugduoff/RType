/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Input.hpp file
*/

#ifndef INPUT_HPP
    #define INPUT_HPP

    #include <SFML/Graphics.hpp>
    #include <SFML/Window.hpp>
    #include <libconfig.h++>
    #include "GameEngine/GameEngine.hpp"

namespace Systems {
    class Input {
        public:

            Input();
            Input(libconfig::Setting &config);
            ~Input() = default;

            void run(Engine::GameEngine &engine, sf::RenderWindow &window);
            void init(Engine::GameEngine &engine, sf::RenderWindow &window);

        private:

            void handleInput(Engine::GameEngine &engine, sf::Event &event);
    };
};

#endif // INPUT_HPP
