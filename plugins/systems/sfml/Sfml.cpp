/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Sfml.cpp file
*/

#define NOMINMAX
#include "GameEngine/GameEngine.hpp"
#include "components/position/Position.hpp"
#include "components/spriteId/SpriteID.hpp"
#include "SpriteComponent.hpp"
#include "Sfml.hpp"
#include "library_entrypoint.hpp"

Systems::Sfml::Sfml(libconfig::Setting &config) :
    displaySystem(config),
    inputSystem(config),
    menu(config),
    __music()
{
    int width;
    int height;
    std::string title;

    config.lookupValue("width", width);
    config.lookupValue("height", height);
    config.lookupValue("title", title);

    __window.create(sf::VideoMode(width, height), title);
    sf::View view(sf::FloatRect(0, 0, 1920, 1080));
    __window.setView(view);
}

Systems::Sfml::Sfml() :
    displaySystem(),
    inputSystem(),
    menu(),
    __window(sf::VideoMode(1920, 1080, 32), "RType"),
    __music()
{
    sf::View view(sf::FloatRect(0, 0, 1920, 1080));
    __window.setView(view);
}

void Systems::Sfml::init(Engine::GameEngine &engine)
{
    std::cout << "Init Sfml" << std::endl;
    displaySystem.init(engine);
    inputSystem.init(engine);
    menu.init(engine);
    std::cout << "Init Sfml done" << std::endl;
    if (!__music.openFromFile("./assets/background.mp3")) {
        std::cerr << "Failed to load music" << std::endl;
        return;
    } else {
        std::cerr << "Music loaded" << std::endl;
        __music.play();
        __music.setLoop(true);
    }
}

void Systems::Sfml::run(Engine::GameEngine &engine)
{
    displaySystem.run(engine, __window);
    inputSystem.run(engine, __window);
    menu.run(engine, __window);
    if (!__window.isOpen()) {
        std::cout << "Send exit" << std::endl;
        exit(0);
    }
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
