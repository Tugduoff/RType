/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Collider.cpp file
*/

#include "Collider.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
char const *const componentName = "Collider";

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault()
{
    return new Components::Collider();
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(uint32_t width, uint32_t height)
{
    return new Components::Collider(width, height);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Collider(config);
}
