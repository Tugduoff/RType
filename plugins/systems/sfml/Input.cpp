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

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            std::cerr << "Window closed" << std::endl;
            window.close();
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                std::cerr << "Escape pressed" << std::endl;
                window.close();
            }
        }
        handleInput(engine, event);
    }
}

void Systems::Input::handleInput(Engine::GameEngine &engine, sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Z) {
            std::cerr << "Z key pressed" << std::endl;
        }
        if (event.key.code == sf::Keyboard::Q) {
            std::cerr << "Q key pressed" << std::endl;
        }
        if (event.key.code == sf::Keyboard::S) {
            std::cerr << "S key pressed" << std::endl;
        }
        if (event.key.code == sf::Keyboard::D) {
            std::cerr << "D key pressed" << std::endl;
        }
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            std::cerr << "Left mouse button pressed" << std::endl;
        }
        if (event.mouseButton.button == sf::Mouse::Right) {
            std::cerr << "Right mouse button pressed" << std::endl;
        }
    }
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            std::cerr << "Left mouse button released" << std::endl;
        }
        if (event.mouseButton.button == sf::Mouse::Right) {
            std::cerr << "Right mouse button released" << std::endl;
        }
    }
}
