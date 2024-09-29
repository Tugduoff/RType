/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Velocity.cpp file
*/

#include <memory>
#include "Velocity.hpp"

extern "C" std::unique_ptr<Components::IComponent> entryPoint(uint32_t x, uint32_t y)
{
    return std::make_unique<Components::Velocity>(x, y);
}
