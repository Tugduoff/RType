/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Display.cpp file
*/

#define NOMINMAX
#include "components/position/Position.hpp"
#include "components/spriteId/SpriteID.hpp"
#include "components/scale/Scale.hpp"
#include "SpriteComponent.hpp"
#include "Display.hpp"

Systems::Display::Display(libconfig::Setting &)
{
}

Systems::Display::Display()
{
}

void Systems::Display::init(Engine::GameEngine &engine, sf::RenderWindow &window)
{
    auto &manager = engine.getRegistry().componentManager();

    window.clear();
    if (!engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position"))
        std::cerr << "Error: Could not register Position component in system Display" << std::endl;
    if (!engine.registerComponent<Components::SpriteIDComponent>("./plugins/bin/components/", "SpriteID"))
        std::cerr << "Error: Could not register SpriteID component in system Display" << std::endl;
    if (!engine.registerComponent<Components::Scale>("./plugins/bin/components/", "Scale"))
        std::cerr << "Error: Could not register Scale component in system Display" << std::endl;
    auto ctor = []() -> Components::SpriteComponent * { return new Components::SpriteComponent(); };
    if (!manager.registerComponent<Components::SpriteComponent>(ctor))
        std::cerr << "Error: Could not register SpriteComponent component in system Display" << std::endl;

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("./assets/player.png")) {
        std::cerr << "Error: Failed to load playerTexture from file" << std::endl;
    }
    sf::Texture enemyTexture;
    if (!enemyTexture.loadFromFile("./assets/enemy.png")) {
        std::cerr << "Error: Failed to load enemyTexture from file" << std::endl;
    }
    sf::Texture playerBullet;
    if (!playerBullet.loadFromFile("./assets/shot1.png")) {
        std::cerr << "Error: Failed to load playerBullet from file" << std::endl;
    }
    sf::Texture enemyBullet;
    if (!enemyBullet.loadFromFile("./assets/shot2.png")) {
        std::cerr << "Error: Failed to load enemyBullet from file" << std::endl;
    }

    __textures.push_back(playerTexture);
    __textures.push_back(enemyTexture);
    __textures.push_back(playerBullet);
    __textures.push_back(enemyBullet);
}

void Systems::Display::run(Engine::GameEngine &engine, sf::RenderWindow &window)
{
    auto &reg = engine.getRegistry();

    window.clear();

    try {
        std::cerr << "DisplaySystem running" << std::endl;
        auto &posComponents = reg.componentManager().getComponents<Components::Position>();
        auto &spriteComponents = reg.componentManager().getComponents<Components::SpriteComponent>();
        auto &spriteIdComponents = reg.componentManager().getComponents<Components::SpriteIDComponent>();
        auto &scaleComponents = reg.componentManager().getComponents<Components::Scale>();

        size_t i = 0;
        for (int l = 0; l < 10; l++) {
            for (i = 0;
                i < posComponents.size() &&
                i < spriteIdComponents.size();
                i++)
            {
                try {
                    auto &pos = posComponents[i];
                    auto &spriteId = spriteIdComponents[i];
                    (void)pos;
                    (void)spriteId;
                } catch (std::exception &e) {
                    std::cerr << "Display err: " << e.what() << std::endl;
                    continue;
                }

                auto &pos = posComponents[i];
                auto &spriteId = spriteIdComponents[i];

                if ((int)pos->layer != l)
                    continue;
                try {
                    auto &sprite = spriteComponents[i];
                    (void)sprite;
                } catch (std::exception &) {
                    std::cerr << "Error: Sprite component not found for this entity, creating it..." << std::endl;
                    std::unique_ptr<Components::SpriteComponent> spriteComp = std::make_unique<Components::SpriteComponent>();
                    reg.componentManager().addComponent<Components::SpriteComponent>((ECS::Entity)i, std::move(spriteComp));
                    break;
                }
                std::cerr << "Entity " << i << " has a sprite component" << std::endl;
                auto &sprite = spriteComponents[i];
                std::cerr << "yu" << std::endl;

                sprite->sprite.setPosition(pos->x, pos->y);
                if (sprite->textureLoaded) {
                    window.draw(sprite->sprite);
                    continue;
                }
                if (spriteId->id == Components::SpriteID::Player) {
                    sprite->sprite.setTexture(__textures[0]);
                } else if (spriteId->id == Components::SpriteID::Enemy) {
                    sprite->sprite.setTexture(__textures[1]);
                } else if (spriteId->id == Components::SpriteID::ProjectileRight) {
                    sprite->sprite.setTexture(__textures[2]);
                } else if (spriteId->id == Components::SpriteID::ProjectileLeft) {
                    sprite->sprite.setTexture(__textures[3]);
                } else {
                    std::cerr << "Error: Unknown sprite ID" << std::endl;
                    continue;
                }
                sprite->textureLoaded = true;
                sf::IntRect textrect = sprite->sprite.getTextureRect();
                sprite->sprite.setOrigin(textrect.width / 2, textrect.height / 2);
                try {
                    auto &scale = scaleComponents[i];
                    sprite->sprite.setScale((float)scale->width / 100, (float)scale->height / 100);
                } catch (std::exception &) {
                    sprite->sprite.setScale(1, 1);
                }
                window.draw(sprite->sprite);
            }
        }
        std::cerr << "End of display system" << std::endl;
    } catch (std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    window.display();
}
