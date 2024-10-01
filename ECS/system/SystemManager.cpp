/*
** EPITECH PROJECT, 2024
** RType
** File description:
** SystemManager.cpp file
*/

#include "SystemManager.hpp"
#include "GameEngine.hpp"

void ECS::SystemManager::addSystem(std::unique_ptr<Systems::ISystem> system)
{
    __systems.push_back(std::move(system));
}

void ECS::SystemManager::run(Engine::GameEngine &engine)
{
    for (auto &system : __systems) {
        system->run(engine);
    }
}

void ECS::SystemManager::initSystems(Engine::GameEngine &engine)
{
    for (auto &system : __systems) {
        if (!system)
            continue;
        system->init(engine);
    }
}
