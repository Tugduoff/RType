/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ConfigLoader.cpp file
*/

#include "GameEngine.hpp"
#include "Config.hpp"
#include <iostream>

Systems::ConfigLoader::ConfigLoader(const std::string &configFilePath) : __configFilePath(configFilePath)
{
}

void Systems::ConfigLoader::init(Engine::GameEngine &engine)
{
    loadConfig(__configFilePath, engine);
}

void Systems::ConfigLoader::run(Engine::GameEngine &engine)
{
    // Do smth

}

std::string Systems::ConfigLoader::difficultyToString(enum Difficulty difficulty)
{
    switch (difficulty) {
        case PEACEFUL:
            return "PEACEFUL";
        case EASY:
            return "EASY";
        case MEDIUM:
            return "MEDIUM";
        case HARD:
            return "HARD";
        case IMPOSSIBLE:
            return "IMPOSSIBLE";
        case GODLIKE:
            return "GODLIKE";
        case NIGHTMARE:
            return "NIGHTMARE";
        case UNREAL:
            return "UNREAL";
    }
}

void Systems::ConfigLoader::displayConfig()
{
    std::cout << "Level name: " << config.level.name << std::endl;
    std::cout << "Level description: " << config.level.description << std::endl;
    std::cout << "Level difficulty: " << difficultyToString(config.level.difficulty) << std::endl;
    std::cout << "Level background music: " << config.level.backgroundMusic << std::endl;
    std::cout << "Level map size: " << config.level.mapSize.x << "x" << config.level.mapSize.y << std::endl;
    std::cout << "Level view port: " << config.level.viewPort.x << "x" << config.level.viewPort.y << std::endl;
    std::cout << "Level cell size: " << config.level.cellSize << std::endl;
    std::cout << "Level goal: " << config.level.goal << std::endl;

    std::cout << "Components: ";
    for (const auto &component : config.components) {
        std::cout << component << " ";
    }
    std::cout << std::endl;

    std::cout << "Systems: ";
    for (const auto &system : config.systems) {
        std::cout << system << " ";
    }
    std::cout << std::endl;

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

void Systems::ConfigLoader::loadConfig(const std::string &filepath, Engine::GameEngine &engine)
{
    try {
        cfg.readFile(filepath);
        std::cout << "Config file loaded: " << filepath << std::endl;
        libconfig::Setting &root = cfg.getRoot();
        extractConfig(root, engine);
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

void Systems::ConfigLoader::extractConfig(libconfig::Setting &root, Engine::GameEngine &engine)
{
        // Extract level information
    if (root.exists("level")) {
        libconfig::Setting &level = root["level"];
        level.lookupValue("name", config.level.name);
        level.lookupValue("description", config.level.description);
        std::string difficultyString;
        level.lookupValue("difficulty", difficultyString);
        config.level.difficulty = difficultyFromString[difficultyString];
        level.lookupValue("background_music", config.level.backgroundMusic);

        // Extract nested structures (map_size, view_port)
        libconfig::Setting &map_size = level["map_size"];
        map_size.lookupValue("x", config.level.mapSize.x);
        map_size.lookupValue("y", config.level.mapSize.y);

        libconfig::Setting &view_port = level["view_port"];
        view_port.lookupValue("x", config.level.viewPort.x);
        view_port.lookupValue("y", config.level.viewPort.y);

        level.lookupValue("cell_size", config.level.cellSize);
        level.lookupValue("goal", config.level.goal);

        std::cout << "Level info: " << config.level.name << " loaded!" << std::endl;
    }

    // Extract entities
    if (root.exists("entities")) {
        libconfig::Setting &entities = root["entities"];
        for (int i = 0; i < entities.getLength(); ++i) {
            ConfigStruct::Entity entity;

            libconfig::Setting &entityComponents = entities[i]["components"];
            for (int j = 0; j < entityComponents.getLength(); ++j) {
                libconfig::Setting &component = entityComponents[j];
                std::string componentName;
                component.lookupValue("name", componentName);

                // Extract the args array
                std::vector<std::any> componentArgs;
                if (component.exists("args")) {
                    libconfig::Setting &args = component["args"];
                    for (int k = 0; k < args.getLength(); ++k) {
                        libconfig::Setting &arg = args[k];
                        if (arg.getType() == libconfig::Setting::TypeInt) {
                            componentArgs.push_back(arg.operator int());
                        } else if (arg.getType() == libconfig::Setting::TypeFloat) {
                            componentArgs.push_back(arg.operator double());
                        } else if (arg.getType() == libconfig::Setting::TypeString) {
                            componentArgs.push_back(arg.operator std::string());
                        } else if (arg.getType() == libconfig::Setting::TypeBoolean) {
                            componentArgs.push_back(arg.operator bool());
                        } else {
                            std::cerr << "Unsupported argument type" << std::endl;
                        }
                    }
                }

                // Store the component in the entity
                entity.components.push_back({componentName, componentArgs});
                std::cout << "Component: " << componentName << " added to entity." << std::endl;
            }
            config.entities.push_back(entity);
        }
        std::cout << "Entities loaded: " << config.entities.size() << std::endl;
    }

    // Create the entities
    for (const auto &entity : config.entities) {
        // Create the entity based on the type and components
        // Add cases for other entity types
        std::cout << "Creating entity of type: " << entity.type << std::endl;
        ECS::Entity newEntity = engine.getRegistry().entityManager().spawnEntity();
        // call engine to create the entity

        for (const auto &component : entity.components) {
            std::shared_ptr<Components::IComponent> comp = engine.getComponentFromId(component.id);
            if (!comp) {
                std::cerr << "Component with ID " << component.id << " not found!" << std::endl;
                continue;  // Skip if the component is not found
            }
            comp->addTo(newEntity, engine, component.args);
        }
    }
}

extern "C" std::unique_ptr<Systems::ISystem> entryPoint(const char *configFilePath)
{
    std::cout << "entryPoint called with configFilePath: " << configFilePath << std::endl;
    return std::make_unique<Systems::ConfigLoader>(configFilePath);
}
