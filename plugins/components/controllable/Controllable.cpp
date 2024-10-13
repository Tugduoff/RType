/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Controllable.cpp file
*/

#include "Controllable.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault()
{
    return new Components::Controllable();
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(std::unordered_map<enum Action, enum Key> &keyBindings)
{
    return new Components::Controllable(keyBindings);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Controllable(config);
}

LIBRARY_ENTRYPOINT
char const *const componentName = "Controllable";
