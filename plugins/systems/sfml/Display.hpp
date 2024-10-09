/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Display.hpp file
*/

#ifndef DISPLAY_HPP
    #define DISPLAY_HPP

    #include <SFML/Graphics.hpp>
    #include <libconfig.h++>
    #include "systems/ASystem.hpp"

namespace Systems {
    class Display : public ASystem {
        public:

            Display();
            Display(libconfig::Setting &config);
            ~Display() = default;

            void run(Engine::GameEngine &engine) override;
            void init(Engine::GameEngine &engine) override;

        private:

            sf::RenderWindow __window;
            std::vector<sf::Texture> __textures;
    };
};

#endif // DISPLAY_HPP
