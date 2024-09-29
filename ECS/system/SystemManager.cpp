/*
** EPITECH PROJECT, 2024
** RType
** File description:
** SystemManager.cpp file
*/

#include "SystemManager.hpp"

void ECS::SystemManager::addSystem(std::unique_ptr<Systems::ISystem> system)
{
    __systems.push_back(std::move(system));
}

void ECS::SystemManager::run(Registry &reg)
{
    for (auto &system : __systems) {
        system->run(reg);
    }
}
