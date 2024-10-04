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

extern "C"
{
    WINDOWS_DLL_EXPORT
    Components::IComponent *entryPoint(uint32_t x, uint32_t y, uint32_t layer)
    {
        return new Components::Position(x, y, layer);
    }

    WINDOWS_DLL_EXPORT
    Components::IComponent *entryConfig(libconfig::Setting &config)
    {
        return new Components::Position(config);
    }

    WINDOWS_DLL_EXPORT
    char const *entryID()
    {
        return "Position";
    }
}

Components::Position::Position(libconfig::Setting &config)
{
    config.lookupValue("x", x);
    config.lookupValue("y", y);
}
