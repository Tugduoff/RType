/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Visible.cpp file
*/

#include "Visible.hpp"
#include "library_entrypoint.hpp"

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

LIBRARY_ENTRYPOINT
char const *componentName = "Visible";

Components::Visible::Visible(libconfig::Setting &config)
{
    if (!config.lookupValue("isVisible", isVisible)) {
        throw std::runtime_error("Missing 'isVisible' setting for Visible component");
    }
}
