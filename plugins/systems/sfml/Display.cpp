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

Systems::Display::Display(libconfig::Setting &config)
{
    if (!config.lookupValue("texturesPath", __configFilePath)) {
        std::cerr << "Error: Could not find configFilePath for all textures in Display config" << std::endl;
    }
    loadConfig(__configFilePath);
}

void Systems::Display::loadConfig(const std::string &filepath)
{
    libconfig::Config cfg;

    try {
        cfg.readFile(filepath);
        std::cout << "\nConfig file loaded: " << filepath << std::endl;
        libconfig::Setting &root = cfg.getRoot();
        libconfig::Setting &textures = root["textures"];
        for (int i = 0; i < textures.getLength(); i++) {
            std::string texturePath;
            textures[i].lookupValue("path", texturePath);
            std::string textureName;
            textures[i].lookupValue("name", textureName);
            std::cerr << "Loading texture: " << textureName << " from path: " << texturePath << std::endl;
            sf::Texture texture;
            if (!texture.loadFromFile(texturePath)) {
                std::cerr << "Error: Could not load texture " << textureName << "from path: " << texturePath << std::endl;
                continue;
            }
            __textures.insert({textureName, texture});
        }
    } catch (libconfig::ParseException &e) {
        std::cerr << "Error while parsing file: "
            << e.getFile() << " in line: "
            << e.getLine() << " : "
            << e.getError() << std::endl;
    } catch (libconfig::FileIOException &e) {
        std::cerr << "Error while reading file: "
            << e.what() << std::endl;
        std::cerr << "File path was: " << filepath << std::endl;
    }
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
    if (!engine.registerComponent<Components::SpriteID>("./plugins/bin/components/", "SpriteID"))
        std::cerr << "Error: Could not register SpriteID component in system Display" << std::endl;
    if (!engine.registerComponent<Components::Scale>("./plugins/bin/components/", "Scale"))
        std::cerr << "Error: Could not register Scale component in system Display" << std::endl;
    auto ctor = []() -> Components::SpriteComponent * { return new Components::SpriteComponent(); };
    if (!manager.registerComponent<Components::SpriteComponent>(ctor))
        std::cerr << "Error: Could not register SpriteComponent component in system Display" << std::endl;
}

void Systems::Display::run(Engine::GameEngine &engine, sf::RenderWindow &window)
{
    auto &reg = engine.getRegistry();

    window.clear();

    try {
        auto &posComponents = reg.componentManager().getComponents<Components::Position>();
        auto &spriteComponents = reg.componentManager().getComponents<Components::SpriteComponent>();
        auto &spriteIdComponents = reg.componentManager().getComponents<Components::SpriteID>();
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
                auto &sprite = spriteComponents[i];

                sprite->sprite.setPosition(pos->x, pos->y);
                if (sprite->textureLoaded) {
                    window.draw(sprite->sprite);
                    continue;
                }
                if (__textures.find(spriteId->id) == __textures.end()) {
                    std::cerr << "Error: Could not find texture with name: " << spriteId->id << std::endl;
                    continue;
                }
                sprite->sprite.setTexture(__textures[spriteId->id]);
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
    } catch (std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    window.display();
}
