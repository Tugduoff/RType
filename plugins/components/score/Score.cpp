/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Score.cpp file
*/

#include "Score.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
char const *const componentName = "Score";

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault()
{
    return new Components::Score();
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(uint32_t value)
{
    return new Components::Score(value);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Score(config);
}
