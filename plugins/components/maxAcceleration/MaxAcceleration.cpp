/*
** EPITECH PROJECT, 2024
** RType
** File description:
** MaxAcceleration.cpp file
*/

#include "MaxAcceleration.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault()
{
    return new Components::MaxAcceleration();
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(int32_t forward, int32_t backward, int32_t right, int32_t left)
{
    return new Components::MaxAcceleration(forward, backward, right, left);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::MaxAcceleration(config);
}

LIBRARY_ENTRYPOINT
char const *const componentName = "MaxAcceleration";
