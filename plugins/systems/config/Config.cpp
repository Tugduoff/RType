/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ConfigLoader.cpp file
*/

#include "GameEngine/GameEngine.hpp"
#include "library_entrypoint.hpp"
#include "Config.hpp"
#include <exception>
#include <iostream>
#include "components/sound/Sound.hpp"

Systems::ConfigLoader::ConfigLoader(const std::string &configFilePath) :
    __configFilePath(configFilePath),
    __chrono()
{
}

Systems::ConfigLoader::ConfigLoader(libconfig::Setting &config) :
    __chrono()
{
    config.lookupValue("path", __configFilePath);
}

void Systems::ConfigLoader::init(Engine::GameEngine &)
{
    loadConfig(__configFilePath);
}

void Systems::ConfigLoader::run(Engine::GameEngine &engine)
{
    for (auto it = config.entities.begin(); it != config.entities.end(); ) {
        auto &entity = *it;

        if ((size_t)entity.spawnTime <= __chrono.getElapsedTime()) {
            // std::cerr << "Spawning entity of type: " << entity.type << " with name: " << entity.name << std::endl;
            ECS::Entity newEntity = engine.getRegistry().createEntity();
            // std::cerr << "New entity created with ID: " << newEntity << std::endl;

            for (const auto &component : entity.components) {
                // std::cerr << "Adding component: \"" << component.id << "\" to entity ID: " << newEntity << std::endl;
                try {
                    std::unique_ptr<Components::IComponent> &comp = engine.getComponentFromId(component.id);
                    comp->addTo(newEntity, engine, component.args);
                } catch (std::exception &e) {
                    std::cerr << "Component with ID: \"" << component.id << "\" not found!" << std::endl;
                    continue;
                }
            }
            try {
                // get the sound component of the entity
                auto &sound = engine.getRegistry().componentManager().getComponents<Components::Sound>()[newEntity];

                for (auto &soundInstance : sound->sounds) {
                    if (std::get<0>(soundInstance) == "SPAWN" || std::get<0>(soundInstance) == "AMBIENT") {
                        std::get<5>(soundInstance) = true;
                        // engine.updateComponent((ECS::Entity)newEntity, sound->getId(), sound->serialize());
                    }
                }
            } catch (std::exception &) {}

            it = config.entities.erase(it);
        } else
            ++it;
    }
}

void Systems::ConfigLoader::displayConfig()
{
    std::cout << "Entity templates: " << std::endl;
    for (const auto &entityTemplate : config.entityTemplates) {
        std::cout << "Template: " << entityTemplate.first << std::endl;
        for (const auto &component : entityTemplate.second.components) {
            std::cout << typeid(component).name() << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Entities: " << std::endl;
    for (const auto &entity : config.entities) {
        std::cout << "Entity type: " << entity.type << std::endl;
        for (const auto &component : entity.components) {
            std::cout << typeid(component).name() << " ";
        }
        std::cout << std::endl;
    }
}

void Systems::ConfigLoader::loadConfig(const std::string &filepath)
{
    try {
        cfg.readFile(filepath);
        std::cout << "\nConfig file loaded: " << filepath << std::endl;
        libconfig::Setting &root = cfg.getRoot();
        extractConfig(root);
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

void Systems::ConfigLoader::extractConfig(libconfig::Setting &root)
{
    if (root.exists("templates")) {
        libconfig::Setting &templates = root["templates"];
        for (int i = 0; i < templates.getLength(); ++i) {
            ConfigStruct::EntityTemplate entityTemplate;
            std::string templateName;
            templates[i].lookupValue("name", templateName);

            libconfig::Setting &templateComponents = templates[i]["components"];
            for (int j = 0; j < templateComponents.getLength(); ++j) {
                libconfig::Setting &component = templateComponents[j];
                std::string componentName;
                component.lookupValue("name", componentName);

                libconfig::Setting &args = component["args"];

                entityTemplate.components.push_back({componentName, args});
            }
            config.entityTemplates.insert({templateName, entityTemplate});
        }
        std::cout << "Templates loaded: " << config.entityTemplates.size() << "\n" << std::endl;
    }

    if (root.exists("entities")) {
        libconfig::Setting &entities = root["entities"];
        for (int i = 0; i < entities.getLength(); ++i) {
            ConfigStruct::Entity entity;
            entity.type = "";
            entity.spawnTime = 0;
            entity.name = "entity";

            entities[i].lookupValue("spawnTime", entity.spawnTime);
            entities[i].lookupValue("type", entity.type);
            entities[i].lookupValue("name", entity.name);

            libconfig::Setting &entityComponents = entities[i]["components"];
            for (int j = 0; j < entityComponents.getLength(); ++j) {
                libconfig::Setting &component = entityComponents[j];
                std::string componentName;
                component.lookupValue("name", componentName);

                libconfig::Setting &args = component["args"];

                entity.components.push_back({componentName, args});
            }
            if (entity.type != "" && entity.type != "Unknown") {
                std::cerr << "Entity type: " << entity.type << " name: " << entity.name << " loaded!" << std::endl;
                for (const auto &component : config.entityTemplates.at(entity.type).components) {
                    bool alreadyLoaded = false;
                    for (const auto &loadedComponent : entity.components) {
                        if (loadedComponent.id == component.id) {
                            std::cerr << "Component: " << component.id << " already defined locally!" << std::endl;
                            alreadyLoaded = true;
                        }
                    }
                    if (alreadyLoaded)
                        continue;
                    entity.components.push_back(component);
                    std::cerr << "Component: " << component.id << " loaded!" << std::endl;
                }
            } else {
                std::cerr << "ERROR: Entity type not found! Entity doesnt depend on a template" << std::endl;
            };
            config.entities.push_back(entity);
        }
        std::cout << "Entities loaded: " << config.entities.size() << "\n" << std::endl;
    }
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryPoint(const char *configFilePath)
{
    return new Systems::ConfigLoader(configFilePath);
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryConfig(libconfig::Setting &config)
{
    return new Systems::ConfigLoader(config);
}
