/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Velocity.cpp file
*/

#include <memory>
#include <iostream>
#include <stdexcept>
#include "Velocity.hpp"

extern "C"
{
    WINDOWS_DLL_EXPORT
    Components::IComponent *entryPoint(uint32_t x, uint32_t y)
    {
        return new Components::Velocity(x, y);
    }

    WINDOWS_DLL_EXPORT
    Components::IComponent *entryConfig(libconfig::Setting &config)
    {
        return new Components::Velocity(config);
    }

    WINDOWS_DLL_EXPORT
    char const *entryID()
    {
        return "Velocity";
    }
}

Components::Velocity::Velocity(libconfig::Setting &config)
{
    config.lookupValue("x", x);
    config.lookupValue("y", y);
}
