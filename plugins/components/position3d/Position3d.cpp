/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Position3d.cpp file
*/

#include "Position3d.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault()
{
    return new Components::Position3d();
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(int32_t x, int32_t y, int32_t z)
{
    return new Components::Position3d(x, y, z);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Position3d(config);
}

LIBRARY_ENTRYPOINT
char const *const componentName = "Position3d";
