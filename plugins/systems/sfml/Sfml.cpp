/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Sfml.cpp file
*/

#include "GameEngine/GameEngine.hpp"
#include "components/position/Position.hpp"
#include "components/spriteId/SpriteID.hpp"
#include "SpriteComponent.hpp"
#include "Sfml.hpp"
#include "library_entrypoint.hpp"

Systems::Sfml::Sfml(libconfig::Setting &config) :
    displaySystem(config),
    inputSystem(config),
    __window(sf::VideoMode(1920, 1080), "RType")
{
    // int width;
    // int height;
    // std::string title;

    // config.lookupValue("width", width);
    // config.lookupValue("height", height);
    // config.lookupValue("title", title);

    // std::cout << "Width: " << width << " Height: " << height << " Title: " << title << std::endl;
    // __window.create(sf::VideoMode(width, height), title);
}

Systems::Sfml::Sfml() :
    displaySystem(),
    inputSystem(),
    __window(sf::VideoMode(1920, 1080), "RType")
{
}

void Systems::Sfml::init(Engine::GameEngine &engine)
{
    std::cout << "Init Sfml" << std::endl;
    displaySystem.init(engine, __window);
    inputSystem.init(engine, __window);
    std::cout << "Init Sfml done" << std::endl;
}

void Systems::Sfml::run(Engine::GameEngine &engine)
{
    std::cout << "Run Sfml" << std::endl;
    displaySystem.run(engine, __window);
    inputSystem.run(engine, __window);
    if (!__window.isOpen())
        exit(0);
    std::cout << "Run Sfml done" << std::endl;
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryPoint()
{
    return new Systems::Sfml();
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryConfig(libconfig::Setting &config)
{
    return new Systems::Sfml(config);
}
