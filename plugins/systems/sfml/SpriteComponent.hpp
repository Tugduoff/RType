/*
** EPITECH PROJECT, 2024
** RType
** File description:
** SpriteComponent.hpp file
*/

#include <SFML/Graphics/Sprite.hpp>

// TODO choose an appropriate namespace

namespace Components {
    class SpriteComponent {
        public:

            SpriteComponent() : sprite(), textureLoaded(false) {};

            sf::Sprite sprite;
            bool textureLoaded;
    };
};
