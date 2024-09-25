/*
** EPITECH PROJECT, 2024
** RType
** File description:
** SystemManager.cpp file
*/

#include "SystemManager.hpp"

void ECS::SystemManager::addSystem(ISystem *system)
{
    __systems.push_back(system);
}

void ECS::SystemManager::addSystem(const ISystem *system)
{
    __systems.push_back(const_cast<ISystem *>(system));
}

void ECS::SystemManager::run(Registry &reg)
{
    for (auto system : __systems) {
        system->func(reg);
    }
}
