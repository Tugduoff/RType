/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Projectile.cpp file
*/

#include "Projectile.hpp"
#include "components/velocity/Velocity.hpp"
#include "components/position/Position.hpp"
#include "components/collider/Collider.hpp"
#include "components/damage/Damage.hpp"
#include "components/spriteId/SpriteID.hpp"
#include "components/deathRange/DeathRange.hpp"
#include "utils/ComponentUtils.hpp"

void createProjectile(
    Engine::GameEngine &engine, 
    int posX, int posY, 
    int velX, int velY, 
    int colliderWidth,
    int colliderHeight, 
    int damageValue,
    enum Components::TypeID type,
    const std::string &spriteID)
{
    auto &reg = engine.getRegistry();
    ECS::Entity projectileEntity = reg.createEntity();

    std::cerr << "Created new projectileEntity with id: " << projectileEntity << std::endl;

    attachAndUpdateComponent<Components::Position>(engine, projectileEntity, posX, posY, 1);
    attachAndUpdateComponent<Components::Velocity>(engine, projectileEntity, velX, velY, 100);
    attachAndUpdateComponent<Components::Collider>(engine, projectileEntity, colliderWidth, colliderHeight);
    attachAndUpdateComponent<Components::Damage>(engine, projectileEntity, damageValue);
    attachAndUpdateComponent<Components::Type>(engine, projectileEntity, type);
    attachAndUpdateComponent<Components::SpriteID>(engine, projectileEntity, spriteID);
    attachAndUpdateComponent<Components::DeathRange>(engine, projectileEntity, 1920, 1080, 0, 0);
}
