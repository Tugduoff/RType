/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Menu.hpp file
*/

#ifndef MENU_HPP
    #define MENU_HPP

    #include <libconfig.h++>
    #include "systems/ASystem.hpp"
    #include <vector>
    #include <SFML/Graphics.hpp>

namespace Engine {
    class GameEngine;
}

namespace Systems {

    class Menu {
        public:
            Menu() = default;
            Menu(libconfig::Setting &config);
            ~Menu() = default;

            void run(Engine::GameEngine &engine, sf::RenderWindow &window);
            void init(Engine::GameEngine &engine);

            size_t getIndexInVector(std::vector<size_t> &vec, size_t &e);
            void destroyEntity(Engine::GameEngine &engine);

            std::vector<ECS::Entity> _entities;
        
        private:
            bool _titleReachedLeftEdge;
            bool _titleDeployed;
            bool _tmSpawned;
            std::vector<size_t> __controllableTexts;
            size_t __selectedText;
            size_t __selectManager;
    };
};

#endif // MENU_HPP_
