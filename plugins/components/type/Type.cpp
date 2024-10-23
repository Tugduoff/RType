/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Type.cpp file
*/

#include "Type.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(enum Components::TypeID &id)
{
    return new Components::Type(id);
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault()
{
    return new Components::Type();
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Type(config);
}

LIBRARY_ENTRYPOINT
char const *const componentName = "Type";
