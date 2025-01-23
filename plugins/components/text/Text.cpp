/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Text.cpp file
*/

#include "Text.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(std::string value, enum Components::TextID &id)
{
    return new Components::Text(value, id);
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault()
{
    return new Components::Text();
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Text(config);
}

LIBRARY_ENTRYPOINT
char const *const componentName = "Text";
