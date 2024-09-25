/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Velocity.cpp file
*/

#include "Velocity.hpp"

extern "C" IComponent *entryPoint(uint32_t x, uint32_t y)
{
    return new Velocity(x, y);
}