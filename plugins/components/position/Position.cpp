/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Position.cpp file
*/

#include "Position.hpp"

extern "C" IComponent *entryPoint(uint32_t x, uint32_t y)
{
    return new Position(x, y);
}
