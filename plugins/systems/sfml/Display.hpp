/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Display.hpp file
*/

#include <SFML/Graphics/RenderWindow.hpp>
#include "systems/ASystem.hpp"

namespace Systems {
    class Display : public ASystem {
    public:

    private:
        sf::RenderWindow __window;
    };
}