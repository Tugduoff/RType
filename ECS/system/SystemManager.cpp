/*
** EPITECH PROJECT, 2024
** RType
** File description:
** SystemManager.cpp file
*/

#include "SystemManager.hpp"
#include "plugins/systems/ISystem.hpp"

void ECS::SystemManager::addSystem(Systems::ISystem *system)
{
    __systems.push_back(system);
}

void ECS::SystemManager::addSystem(const Systems::ISystem *system)
{
    __systems.push_back(const_cast<Systems::ISystem *>(system));
}

void ECS::SystemManager::run(Registry &reg)
{
    for (Systems::ISystem *system : __systems) {
        system->run(reg);
    }
}
