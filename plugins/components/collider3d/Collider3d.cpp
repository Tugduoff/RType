/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Collider3d.cpp file
*/

#include "Collider3d.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
char const *const componentName = "Collider3d";

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault()
{
    return new Components::Collider3d();
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(uint32_t width, uint32_t height, uint32_t depth)
{
    return new Components::Collider3d(width, height, depth);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Collider3d(config);
}
