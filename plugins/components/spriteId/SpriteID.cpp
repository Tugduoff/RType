/*
** EPITECH PROJECT, 2024
** RType
** File description:
** SpriteID.cpp file
*/

#include "SpriteID.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(std::string id)
{
    return new Components::SpriteID(id);
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault()
{
    return new Components::SpriteID();
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::SpriteID(config);
}

LIBRARY_ENTRYPOINT
char const *const componentName = "SpriteID";
