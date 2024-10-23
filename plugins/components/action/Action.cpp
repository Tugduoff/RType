/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Action.cpp file
*/

#include "Action.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault()
{
    return new Components::ActionComponent();
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams()
{
    return new Components::ActionComponent();
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::ActionComponent(config);
}

LIBRARY_ENTRYPOINT
char const *const componentName = "Action";
