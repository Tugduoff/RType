/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Display.cpp file
*/

#include "ECS/utilities/Zipper/IndexedZipper.hpp"
#define NOMINMAX
#include "components/position/Position.hpp"
#include "components/spriteId/SpriteID.hpp"
#include "Text.hpp"
#include "components/scale/Scale.hpp"
#include "SpriteComponent.hpp"
#include "Display.hpp"
#include <iostream>

Systems::Display::Display(libconfig::Setting &config)
    : __shaders(), __currentShader(nullptr)
{
    libconfig::Config cfg;
    std::string shadersPath;
    std::string defaultShader = "default";

    if (!config.lookupValue("texturesPath", __configFilePath))
        throw("Can't find configFilePath for all textures in Display config");
    if (!config.lookupValue("shadersPath", shadersPath))
        throw("Can't find shadersPath in Display config");
    if (!config.lookupValue("defaultShader", defaultShader))
        throw("Can't find defaultShader in Display config");

    loadConfig(__configFilePath);

    try {
        cfg.readFile(shadersPath);
        std::cerr << "\nConfig file loaded: " << shadersPath << std::endl;
        libconfig::Setting &root = cfg.getRoot();

        libconfig::Setting &shaders = root["shaders"];
        for (int i = 0; i < shaders.getLength(); ++i) {
            __shaders.emplace_back(std::make_shared<Shader>(shaders[i]));
        }
        if (__shaders.empty()) {
            std::cerr << "No shaders loaded" << std::endl;
            return;
        }
        std::cerr << "Shaders loaded: " << __shaders.size() << std::endl;
        if (defaultShader != "default") {
            for (auto &shader : __shaders) {
                if (shader->name == defaultShader) {
                    __currentShader = shader;
                    break;
                }
            }
        }
    } catch (libconfig::ParseException &e) {
        std::string err = "Error while parsing file: " + std::string(e.getFile()) + " in line: " + std::to_string(e.getLine()) + " : " + std::string(e.getError());
        throw std::runtime_error(err);
    } catch (libconfig::FileIOException &e) {
        std::string err = "Error while reading file: " + std::string(e.what()) + "\n" + "File path was: " + shadersPath;
        throw std::runtime_error(err);
    }
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
            std::string type;
            std::string texturePath;
            std::string textureName;
            std::string actionName;
            sf::IntRect rect;
            unsigned updateRate = 0;
            unsigned rectX = 0;
            unsigned rectY = 0;

            textures[i].lookupValue("type", type);
            textures[i].lookupValue("name", textureName);
            if (type == "static") {
                textures[i].lookupValue("path", texturePath);

                std::vector<Texture> textures;

                sf::Texture tempTexture;
                if (!tempTexture.loadFromFile(texturePath)) {
                    std::cerr << "Error: Could not load texture from path: " << texturePath << std::endl;
                }
                Texture texture(0, tempTexture.getSize().x, tempTexture.getSize().y, texturePath, "IDLE", false, false);
                textures.push_back(texture);
                __textures.insert({textureName, textures});
                continue;
            }
            if (type == "animated") {
                bool repeat = false;
                bool resetBeforeEnd = false;

                textures[i].lookupValue("path", texturePath);
                textures[i].lookupValue("updateRate", updateRate);
                textures[i].lookupValue("repeat", repeat);
                textures[i].lookupValue("resetBeforeEnd", resetBeforeEnd);

                libconfig::Setting &rectSetting = textures[i]["textureRect"];
                rectSetting.lookupValue("x", rectX);
                rectSetting.lookupValue("y", rectY);

                std::vector<Texture> textures;
                Texture texture(updateRate, rectX, rectY, texturePath, "IDLE", repeat, resetBeforeEnd);
                textures.push_back(texture);
                __textures.insert({textureName, textures});
                continue;
            }
            if (type == "actions") {
                libconfig::Setting &actions = textures[i]["actions"];
                std::vector<Texture> textures;

                for (int j = 0; j < actions.getLength(); j++) {
                    libconfig::Setting &action = actions[j];
                    action.lookupValue("type", type);

                    if (type == "static") {
                        action.lookupValue("path", texturePath);
                        action.lookupValue("action", actionName);

                        sf::Texture tempTexture;
                        if (!tempTexture.loadFromFile(texturePath)) {
                            std::cerr << "Error: Could not load texture from path: " << texturePath << std::endl;
                        }
                        Texture texture(0, tempTexture.getSize().x, tempTexture.getSize().y, texturePath, actionName, false, false);
                        textures.push_back(texture);
                        continue;
                    } else if (type == "animated") {
                        bool repeat = false;
                        bool resetBeforeEnd = false;

                        action.lookupValue("path", texturePath);
                        action.lookupValue("action", actionName);
                        action.lookupValue("updateRate", updateRate);
                        action.lookupValue("repeat", repeat);
                        action.lookupValue("resetBeforeEnd", resetBeforeEnd);

                        libconfig::Setting &rectSetting = action["textureRect"];
                        rectSetting.lookupValue("x", rectX);
                        rectSetting.lookupValue("y", rectY);
                        Texture texture(updateRate, rectX, rectY, texturePath, actionName, repeat, resetBeforeEnd);
                        textures.push_back(texture);
                        std::cerr << "Successfully loaded texture: " << textureName << " for action: " << actionName << std::endl;
                        continue;
                    }
                }
                __textures.insert({textureName, textures});
            }
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
    : __shaders()
{
}

void Systems::Display::init(Engine::GameEngine &engine)
{
    engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position");
    engine.registerComponent<Components::SpriteID>("./plugins/bin/components/", "SpriteID");
    engine.registerComponent<Components::Scale>("./plugins/bin/components/", "Scale");

    auto &manager = engine.getRegistry().componentManager();
    auto ctor = []() -> Components::SpriteComponent * { return new Components::SpriteComponent(); };
    manager.registerComponent<Components::SpriteComponent>(ctor);

    auto ctorText = []() -> Components::Text * { return new Components::Text(); };
    manager.registerComponent<Components::Text>(ctorText);
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
        auto &textComponents = reg.componentManager().getComponents<Components::Text>();

        for (unsigned l = 0; l < 10; l++) {
            for (auto &&[i, pos, spriteId] : IndexedZipper(posComponents, spriteIdComponents)) {
                if (pos.layer != l || spriteId.id.empty()) {
                    continue;
                }

                try {
                    auto &sprite = spriteComponents[i];
                    sprite->sprite.setPosition(pos.x, pos.y);
                    sprite->update();

                    if (__currentShader && __currentShader->shader.isAvailable()) {
                        window.draw(sprite->sprite, &__currentShader->shader);
                    } else {
                        window.draw(sprite->sprite);
                    }

                    try {
                        auto &scale = scaleComponents[i];
                        sprite->sprite.setScale((float)scale->width / 100, (float)scale->height / 100);
                    } catch (std::exception &) {
                        sprite->sprite.setScale(1, 1);
                    }
                } catch (std::exception &) {
                    // std::cerr << "Error: Sprite component not found for entity: " << i << " spriteID: " << spriteId.id << std::endl;
                    std::unique_ptr<Components::SpriteComponent> spriteComp = std::make_unique<Components::SpriteComponent>();
                    for (auto &texture : __textures[spriteId.id]) {
                        spriteComp->addTexture(texture);
                    }
                    try {
                        auto &scale = scaleComponents[i];
                        spriteComp->sprite.setScale((float)scale->width / 100, (float)scale->height / 100);
                    } catch (std::exception &) {
                        spriteComp->sprite.setScale(1, 1);
                    }
                    reg.componentManager().addComponent<Components::SpriteComponent>((ECS::Entity)i, std::move(spriteComp));
                }
            }

            for (auto &&[i, pos, text] : IndexedZipper(posComponents, textComponents)) {
                if (pos.layer != l) {
                    continue;
                }
                text.setPosition(pos.x, pos.y);
                window.draw(text.sfText);
            }
        }
    } catch (std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    window.display();
}
