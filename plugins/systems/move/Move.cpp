/*
** EPITECH PROJECT, 2024
** RType
** File description:
** MoveSystem.cpp file
*/

#include "Move.hpp"
#include "plugins/components/position/Position.hpp"
#include "ECS/registry/Registry.hpp"

void Systems::MoveSystem::func(ECS::Registry &reg)
{
    auto &posComponents = reg.get_components<Position>();
    
    for (auto &pos : posComponents) {
        pos->x += 1;
        pos->y += 1;
    }
}

extern "C" Systems::ISystem *entryPoint()
{
    return new Systems::MoveSystem();
}
