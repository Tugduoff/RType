/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Velocity3d.cpp file
*/

#include "Velocity3d.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault()
{
    return new Components::Velocity3d();
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(int32_t x, int32_t y, int32_t z, uint8_t factor)
{
    return new Components::Velocity3d(x, y, z, factor);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Velocity3d(config);
}

LIBRARY_ENTRYPOINT
char const *const componentName = "Velocity3d";
