/*
** EPITECH PROJECT, 2024
** RType
** File description:
** EntityManager.cpp file
*/

#include "EntityManager.hpp"
#include <algorithm>
#include <stdexcept>

ECS::Entity ECS::EntityManager::spawnEntity()
{
    __entities.push_back(Entity(__nextEntityId++));
    return __entities.back();
}

ECS::Entity ECS::EntityManager::entityFromIndex(size_t idx)
{
    if (idx >= __entities.size())
        throw std::runtime_error("Entity index out of range");
    return __entities[idx];
}

void ECS::EntityManager::killEntity(Entity const &e)
{
    __entities.erase(std::remove(__entities.begin(), __entities.end(), e), __entities.end());
}
