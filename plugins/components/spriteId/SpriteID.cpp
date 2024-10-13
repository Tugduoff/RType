/*
** EPITECH PROJECT, 2024
** RType
** File description:
** SpriteIDComponent.cpp file
*/

#include "SpriteID.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(uint32_t id)
{
    return new Components::SpriteIDComponent((Components::SpriteID)id);
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault()
{
    return new Components::SpriteIDComponent();
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::SpriteIDComponent(config);
}

LIBRARY_ENTRYPOINT
char const *const componentName = "SpriteID";
