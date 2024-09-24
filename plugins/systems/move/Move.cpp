/*
** EPITECH PROJECT, 2024
** RType
** File description:
** MoveSystem.cpp file
*/

#include "Move.hpp"
#include "Position.hpp"

void MoveSystem::func(Registry &reg)
{
    auto &posComponents = reg.get_components<Position>();
    
    for (auto &pos : posComponents) {
        pos->x += 1;
        pos->y += 1;
    }
}

extern "C" ISystem *entryPoint()
{
    return new MoveSystem();
}
