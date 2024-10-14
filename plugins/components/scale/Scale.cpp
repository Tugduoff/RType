/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Scale.cpp file
*/

#include "Scale.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
char const *const componentName = "Scale";

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault()
{
    return new Components::Scale();
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(uint32_t width, uint32_t height)
{
    return new Components::Scale(width, height);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Scale(config);
}
