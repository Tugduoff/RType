/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Position.cpp file
*/

#include "Position.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault()
{
    return new Components::Position();
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(uint32_t x, uint32_t y, uint32_t layer)
{
    return new Components::Position(x, y, layer);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Position(config);
}

LIBRARY_ENTRYPOINT
char const *const componentName = "Position";
