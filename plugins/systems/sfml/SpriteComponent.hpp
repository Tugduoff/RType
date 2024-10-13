/*
** EPITECH PROJECT, 2024
** RType
** File description:
** SpriteComponent.hpp file
*/

#include <SFML/Graphics/Sprite.hpp>

namespace Components {
    class SpriteComponent {
        public:

            SpriteComponent() : sprite(), textureLoaded(false) {};

            sf::Sprite sprite;
            bool textureLoaded;
    };
};
