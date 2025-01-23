/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ModelId.cpp file
*/

#include "ModelId.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(std::string id)
{
    return new Components::ModelId(id);
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault()
{
    return new Components::ModelId();
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::ModelId(config);
}

LIBRARY_ENTRYPOINT
char const *const componentName = "ModelId";
