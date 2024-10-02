/*
** EPITECH PROJECT, 2024
** RType
** File description:
** SystemManager.cpp file
*/

#include "SystemManager.hpp"
#include "ECS/registry/Registry.hpp"
#include "ECS/component/ComponentRegisterer.hpp"

void ECS::SystemManager::addSystem(std::unique_ptr<Systems::ISystem> system)
{
    __systems.push_back(std::move(system));
}

void ECS::SystemManager::run(ECS::Registry &registry)
{
    for (auto &system : __systems) {
        system->run(registry);
    }
}

void ECS::SystemManager::initSystems(const ECS::ComponentRegisterer &registerer)
{
    for (auto &system : __systems) {
        if (!system)
            continue;
        system->init(registerer);
    }
}
