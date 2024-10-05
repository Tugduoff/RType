/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Position.cpp file
*/

#include <memory>
#include <iostream>
#include <stdexcept>
#include "Position.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
Components::IComponent *entryPoint(uint32_t x, uint32_t y, uint32_t layer)
{
    return new Components::Position(x, y, layer);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Position(config);
}

LIBRARY_ENTRYPOINT
char const *entryID()
{
    return "Position";
}

Components::Position::Position(libconfig::Setting &config)
{
    config.lookupValue("x", x);
    config.lookupValue("y", y);
}
