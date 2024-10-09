/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Display.cpp file
*/

#include "GameEngine/GameEngine.hpp"
#include "components/position/Position.hpp"
#include "components/spriteId/SpriteID.hpp"
#include "SpriteComponent.hpp"
#include "Display.hpp"
#include "library_entrypoint.hpp"

Systems::Display::Display(libconfig::Setting &config)
{
    int width;
    int height;
    std::string title;

    config.lookupValue("width", width);
    config.lookupValue("height", height);
    config.lookupValue("title", title);
    __window.create(sf::VideoMode(width, height), title);
}

Systems::Display::Display()
{
    __window.create(sf::VideoMode(1920, 1080), "RType");
}

void Systems::Display::init(Engine::GameEngine &engine)
{
    auto &manager = engine.getRegistry().componentManager();

    __window.clear();
    if (!engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position"))
        std::cerr << "Error: Could not register Position component in system Display" << std::endl;
    if (!engine.registerComponent<Components::SpriteIDComponent>("./plugins/bin/components/", "SpriteID"))
        std::cerr << "Error: Could not register SpriteID component in system Display" << std::endl;
    if (!manager.registerComponent<Components::SpriteComponent>())
        std::cerr << "Error: Could not register SpriteComponent component in system Display" << std::endl;

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("./assets/player.png")) {
        std::cerr << "Error: Failed to load playerTexture from file" << std::endl;
    }
    sf::Texture enemyTexture;
    if (!enemyTexture.loadFromFile("./assets/enemy.png")) {
        std::cerr << "Error: Failed to load enemyTexture from file" << std::endl;
    }

    __textures.push_back(playerTexture);
    __textures.push_back(enemyTexture);
}

void Systems::Display::run(Engine::GameEngine &engine)
{
    auto &reg = engine.getRegistry();

    __window.clear();

    try {
        auto &posComponents = reg.componentManager().getComponents<Components::Position>();
        auto &spriteComponents = reg.componentManager().getComponents<Components::SpriteComponent>();
        auto &spriteIdComponents = reg.componentManager().getComponents<Components::SpriteIDComponent>();

        size_t i = 0;
        for (i = 0;
            i < posComponents.size() &&
            i < spriteComponents.size() &&
            i < spriteIdComponents.size();
            i++)
        {
            auto &pos = posComponents[i];
            auto &sprite = spriteComponents[i];
            auto &spriteId = spriteIdComponents[i];

            if (!pos || !sprite || !spriteId)
                continue;
            sprite->sprite.setPosition(pos->x, pos->y);
            if (sprite->textureLoaded) {
                __window.draw(sprite->sprite);
                continue;
            }
            if (spriteId->id == Components::SpriteID::Player) {
                sprite->sprite.setTexture(__textures[0]);
            } else if (spriteId->id == Components::SpriteID::Enemy) {
                sprite->sprite.setTexture(__textures[1]);
            }
            sprite->textureLoaded = true;
            __window.draw(sprite->sprite);
        }
    } catch (std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    __window.display();
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryPoint()
{
    return new Systems::Display();
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryConfig(libconfig::Setting &config)
{
    return new Systems::Display(config);
}
