/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Controllable.cpp file
*/

#include "Controllable.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
Components::IComponent *entryPoint()
{
    return new Components::Controllable();
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Controllable(config);
}

LIBRARY_ENTRYPOINT
char const *const componentName = "Controllable";
