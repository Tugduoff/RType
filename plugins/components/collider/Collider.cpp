/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Collider.cpp file
*/

#include "Collider.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
char const *componentName = "Collider";

LIBRARY_ENTRYPOINT
Components::IComponent *entryPoint(float width, float height)
{
    return new Components::Collider(width, height);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Collider(config);
}
