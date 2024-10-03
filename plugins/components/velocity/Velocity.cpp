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
#ifdef _WIN32
    __declspec(dllexport)
#endif
    Components::IComponent *entryPoint(uint32_t x, uint32_t y)
    {
        return new Components::Velocity(x, y);
    }
}

extern "C" std::unique_ptr<Components::IComponent> entryConfig(libconfig::Setting &config)
{
    return std::make_unique<Components::Velocity>(config);
}

extern "C" std::string entryID()
{
    return "Velocity";
}

Components::Velocity::Velocity(libconfig::Setting &config)
{
    config.lookupValue("x", x);
    config.lookupValue("y", y);
}
