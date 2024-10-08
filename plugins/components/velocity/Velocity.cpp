/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Velocity.cpp file
*/

#include "Velocity.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
Components::IComponent *entryPoint(uint32_t x, uint32_t y)
{
    return new Components::Velocity(x, y);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Velocity(config);
}

LIBRARY_ENTRYPOINT
char const *componentName = "Velocity";

Components::Velocity::Velocity(libconfig::Setting &config)
{
    config.lookupValue("x", x);
    config.lookupValue("y", y);
}
