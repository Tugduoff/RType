/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ConfigStruct.hpp file
*/

#ifndef CONFIGSTRUCT_HPP
    #define CONFIGSTRUCT_HPP

    #include <unordered_map>
    #include <vector>
    #include <cstdint>
    #include <libconfig.h++>

enum Difficulty {
    PEACEFUL,
    EASY,
    MEDIUM,
    HARD,
    IMPOSSIBLE,
    GODLIKE,
    NIGHTMARE,
    UNREAL
};

const std::unordered_map<std::string, Difficulty> difficultyFromString = {
    {"PEACEFUL", PEACEFUL},
    {"EASY", EASY},
    {"MEDIUM", MEDIUM},
    {"HARD", HARD},
    {"IMPOSSIBLE", IMPOSSIBLE},
    {"GODLIKE", GODLIKE},
    {"NIGHTMARE", NIGHTMARE},
    {"UNREAL", UNREAL}
};

struct Component {
    std::string id;
    libconfig::Setting &args;
};

struct ConfigStruct {
    // Level information
    struct Level {
        std::string name;
        std::string description; // Description of the level
        std::string goal; // Sub description of the level > Acts as the goal
        enum Difficulty difficulty; // As an enum
        std::string backgroundMusic; // Filepath to background music
        int cellSize;
        struct { int x, y; } mapSize; // Size of the map in cells
        struct { int x, y; } viewPort; // Size of the viewport in pixels
        uint32_t levelSpeed; // Speed of the level in cells / seconds
    } level;

    std::vector<std::string> components; // List of components that will be loaded when their position is close to the viewport
    std::vector<std::string> systems; // List of systems that need to be loaded at the start of the level

    struct EntityTemplate {
        std::vector<Component> components;
    }; // Entity template structure, when adding new components, add them here as well
    std::unordered_map<std::string, EntityTemplate> entityTemplates; // List of entity templates

    struct Entity {
        std::string type; // If the entity is dependant of a template, the type will be the template name
        std::vector<Component> components;
    }; // Entity structure, when adding new components, add them here as well
    std::vector<Entity> entities; // List of entities
};

#endif // CONFIGSTRUCT_HPP
