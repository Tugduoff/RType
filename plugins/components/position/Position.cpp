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

extern "C" Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Position(config);
}

extern "C" char const *entryID()
{
    return "Position";
}

extern "C"
{
#ifdef _WIN32
    __declspec(dllexport)
#endif
    Components::IComponent *entryPoint(uint32_t x, uint32_t y, uint32_t layer)
    {
        return new Components::Position(x, y, layer);
    }
}

Components::Position::Position(libconfig::Setting &config)
{
    config.lookupValue("x", x);
    config.lookupValue("y", y);
}
