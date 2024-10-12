/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Velocity.cpp file
*/

#include "Velocity.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault()
{
    return new Components::Velocity();
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(uint32_t x, uint32_t y)
{
    return new Components::Velocity(x, y);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Velocity(config);
}

LIBRARY_ENTRYPOINT
char const *const componentName = "Velocity";
