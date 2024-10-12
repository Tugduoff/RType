/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Health.cpp file
*/

#include "Health.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
char const *componentName = "Health";

LIBRARY_ENTRYPOINT
Components::IComponent *entryPoint()
{
    return new Components::Health();
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Health(config);
}
