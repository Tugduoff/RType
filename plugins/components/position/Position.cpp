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

extern "C" std::unique_ptr<Components::IComponent> entryPoint(uint32_t x, uint32_t y)
{
    return std::make_unique<Components::Position>(x, y);
}

extern "C" std::string entryID()
{
    return "Position";
}
