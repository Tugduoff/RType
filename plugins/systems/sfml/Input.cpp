/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Input.cpp file
*/

#include "components/position/Position.hpp"
#include "components/spriteId/SpriteID.hpp"
#include "SpriteComponent.hpp"
#include "Input.hpp"

Systems::Input::Input(libconfig::Setting &)
{
}

Systems::Input::Input()
{
}

void Systems::Input::init(Engine::GameEngine &engine, sf::RenderWindow &window)
{
    // Maybe register a controllable component here
}

void Systems::Input::run(Engine::GameEngine &engine, sf::RenderWindow &window)
{
    sf::Event event;

    std::cout << "Running Input" << std::endl;
    window.pollEvent(event);
    if (event.type == sf::Event::Closed) {
        std::cerr << "Window closed" << std::endl;
        window.close();
    }
    std::cout << "Input done" << std::endl;
}
