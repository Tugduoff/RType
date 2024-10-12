/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Collider.cpp file
*/

#include "Collider.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
Components::IComponent *entryPoint(float width, float height)
{
    return new Components::Collider(width, height);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    float width = 0.0f;
    float height = 0.0f;

    if (!config.lookupValue("width", width) || !config.lookupValue("height", height)) {
        throw std::invalid_argument("Failed to retrieve values for 'width' and 'height' from config");
    }

    return new Components::Collider(width, height);
}

LIBRARY_ENTRYPOINT
char const *componentName = "Collider";

Components::Collider::Collider(libconfig::Setting &config)
{
    if (!config.lookupValue("width", width) || !config.lookupValue("height", height)) {
        throw std::invalid_argument("Failed to retrieve values for 'width' and 'height'");
    }
}
