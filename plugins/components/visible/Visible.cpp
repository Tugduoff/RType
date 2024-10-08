/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Visible.cpp file
*/

#include "Visible.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
char const *componentName = "Visible";

LIBRARY_ENTRYPOINT
Components::IComponent *entryPoint(bool isVisible)
{
    return new Components::Visible(isVisible);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Visible(config);
}
