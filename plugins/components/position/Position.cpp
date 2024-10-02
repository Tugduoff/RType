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

extern "C" std::unique_ptr<Components::IComponent> entryPoint(uint32_t x, uint32_t y, uint32_t layer)
{
    return std::make_unique<Components::Position>(x, y, layer);
}

extern "C" std::unique_ptr<Components::IComponent> entryConfig(libconfig::Setting &config)
{
    return std::make_unique<Components::Position>(config);
}

extern "C" std::string entryID()
{
    return "Position";
}

Components::Position::Position(libconfig::Setting &config)
{
    config.lookupValue("x", x);
    config.lookupValue("y", y);
    config.lookupValue("layer", layer);
}
