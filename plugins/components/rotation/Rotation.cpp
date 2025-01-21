/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Rotation.cpp file
*/

#include "Rotation.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault()
{
    return new Components::Rotation();
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(int32_t x, int32_t y, int32_t z)
{
    return new Components::Rotation(x, y, z);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Rotation(config);
}

LIBRARY_ENTRYPOINT
char const *const componentName = "Rotation";
