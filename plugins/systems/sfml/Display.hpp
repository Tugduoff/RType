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
    #include "GameEngine/GameEngine.hpp"
    #include <unordered_map>

namespace Systems {
    /**
     * @class Display
     * 
     * @brief System class from Systems that displays entities.
     * 
     * This system is responsible for displaying entities on the screen.
     */
    class Display {
        public:

            Display();
            Display(libconfig::Setting &config);
            ~Display() = default;

            void run(Engine::GameEngine &engine, sf::RenderWindow &window);
            void init(Engine::GameEngine &engine, sf::RenderWindow &window);

        private:

            void loadConfig(const std::string &filepath);

            std::unordered_map<std::string, sf::Texture> __textures;
            std::string __configFilePath;
    };
};

#endif // DISPLAY_HPP
