/*
** EPITECH PROJECT, 2024
** Rtype
** File description:
** Registry.cpp file
*/

#include "Registry.hpp"
#include "ECS/entity/Entity.hpp"

ECS::Entity ECS::Registry::createEntity()
{
    Entity entity = this->__entityManager.spawnEntity();

    for (auto const &cb : this->__createCallbacks) {
        cb(entity);
    }
    return entity;
}

void ECS::Registry::killEntity(const ECS::Entity &entity)
{
    this->__componentManager.removeAllFromEntity(entity);
    this->__entityManager.killEntity(entity);
    for (auto const &cb : this->__killCallbacks) {
        cb(entity);
    }
}
