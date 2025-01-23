/*
** EPITECH PROJECT, 2024
** RType
** File description:
** MarioType.cpp file
*/

#include "MarioType.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(enum Components::MarioTypeID &id)
{
    return new Components::MarioType(id);
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault()
{
    return new Components::MarioType();
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::MarioType(config);
}

LIBRARY_ENTRYPOINT
char const *const componentName = "MarioType";
