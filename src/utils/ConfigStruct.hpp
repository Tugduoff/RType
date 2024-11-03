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

struct Component {
    std::string id;
    libconfig::Setting &args;
};

struct ConfigStruct {
    /**
     * @brief Entity template structure
     * 
     * This structure is used to store the entity templates that will be used to create entities
     * 
     * @param name The name of the template
     * @param components The list of components that the entity will have
     */
    struct EntityTemplate {
        std::string name;
        std::vector<Component> components;
    }; // Entity template structure, when adding new components, add them here as well
    std::unordered_map<std::string, EntityTemplate> entityTemplates; // List of entity templates

    /**
     * @brief Entity structure
     * 
     * This structure is used to store the entities that will be spawned in the level
     * 
     * @param type The type of the entity, if the entity is dependant of a template, the type will be the template name
     * @param name The name of the entity
     * @param spawnTime The time in milliseconds before the entity spawns
     * @param components The list of components that the entity will have
     */
    struct Entity {
        std::string type; // If the entity is dependant of a template, the type will be the template name
        std::string name; // Name of the entity
        int spawnTime; // Time in milliseconds before the entity spawns
        std::vector<Component> components;
    }; // Entity structure, when adding new components, add them here as well
    std::vector<Entity> entities; // List of entities
};

#endif // CONFIGSTRUCT_HPP
