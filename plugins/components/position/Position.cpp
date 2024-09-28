/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Position.cpp file
*/

#include "Position.hpp"

extern "C" Components::IComponent *entryPoint(uint32_t x, uint32_t y)
{
    return new Components::Position(x, y);
}
