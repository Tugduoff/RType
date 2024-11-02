/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Ai.cpp file
*/

#include "Ai.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
char const *const componentName = "Ai";

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault()
{
    return new Components::Ai();
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(std::string id)
{
    return new Components::Ai(id);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Ai(config);
}
