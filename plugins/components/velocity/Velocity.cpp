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
Components::IComponent *buildWithParams(int32_t x, int32_t y, uint8_t factor)
{
    return new Components::Velocity(x, y, factor);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Velocity(config);
}

LIBRARY_ENTRYPOINT
char const *const componentName = "Velocity";
