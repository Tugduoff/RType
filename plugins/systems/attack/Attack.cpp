/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Attack.cpp file
*/

#include "GameEngine/GameEngine.hpp"
#include "Attack.hpp"
#include "components/gun/Gun.hpp"
#include "EntityActions.hpp"
#include "components/velocity/Velocity.hpp"
#include "components/position/Position.hpp"
#include "components/collider/Collider.hpp"
#include "components/damage/Damage.hpp"
#include "components/spriteId/SpriteID.hpp"
#include "components/deathRange/DeathRange.hpp"
#include "library_entrypoint.hpp"
#include "utils/Projectile.hpp"
#include <iostream>
#include <stdexcept>

Systems::AttackSystem::AttackSystem(libconfig::Setting &)
{
}

void Systems::AttackSystem::run(Engine::GameEngine &engine)
{
    auto &reg = engine.getRegistry();

    try {
        auto &gunComponents = reg.componentManager().getComponents<Components::Gun>();
        auto &posComponents = reg.componentManager().getComponents<Components::Position>();
        auto &typeComponents = reg.componentManager().getComponents<Components::Type>();
        auto &actionComponents = reg.componentManager().getComponents<Components::ActionComponent>();

        size_t i = 0;
        for (i = 0;
            i < gunComponents.size() &&
            i < posComponents.size() &&
            i < typeComponents.size(); i++) {
            try {
                auto &gun = gunComponents[i];
                auto &pos = posComponents[i];
                auto &spr = typeComponents[i];
                (void)gun;
                (void)pos;
                (void)spr;
            } catch (std::exception &e) {
                continue;
            }
            auto &gun = gunComponents[i];
            auto &pos = posComponents[i];
            auto &spr = typeComponents[i];

            if (!gun || !pos)
                continue;
            if (spr->id == Components::TypeID::ALLY)
                continue;
            try {
                auto &entityAction = actionComponents[i];

                if (entityAction->action == EntityAction::SHOOT_FORWARD && gun->chrono.getElapsedTime() > gun->fireRate / 2)
                    entityAction->action = EntityAction::IDLE;
            } catch (std::exception &e) {
                std::unique_ptr<Components::ActionComponent> actionComp = std::make_unique<Components::ActionComponent>();
                engine.getRegistry().componentManager().addComponent<Components::ActionComponent>((ECS::Entity)i, std::move(actionComp));
                std::cerr << "Set default action for entity: " << i << " in ActionManager." << std::endl;
                continue;
            }
            if (gun->chrono.getElapsedTime() < gun->fireRate)
                continue;
            gun->chrono.restart();

            int projectilePosX = pos->x;
            int projectilePosY = pos->y;
            int projectileVelX = gun->bulletVelocity;
            int projectileVelY = 0;
            int projectileColliderWidth = 10;
            int projectileColliderHeight = 10;
            int projectileDamage = gun->bulletDamage;
            enum Components::TypeID type = Components::TypeID::ENEMY_PROJECTILE;
            std::string spriteID = gun->spriteId;

            createProjectile(
                engine,
                projectilePosX,
                projectilePosY,
                projectileVelX,
                projectileVelY,
                projectileColliderWidth,
                projectileColliderHeight,
                projectileDamage,
                type,
                spriteID);

            try {
                auto &entityAction = actionComponents[i];

                entityAction->action = EntityAction::SHOOT_FORWARD;
                std::cerr << "Chaning action to SHOOT_FORWARD for entity: " << i << std::endl;
            } catch (std::exception &e) {
                std::unique_ptr<Components::ActionComponent> actionComp = std::make_unique<Components::ActionComponent>();
                engine.getRegistry().componentManager().addComponent<Components::ActionComponent>((ECS::Entity)i, std::move(actionComp));
                std::cerr << "Set default action for entity: " << i << " in ActionManager." << std::endl;
                continue;
            }
        }
    } catch (std::runtime_error &e) {
        std::cerr << "Attack Error: " << e.what() << std::endl;
    }
}

void Systems::AttackSystem::init(Engine::GameEngine &engine)
{
    if (!engine.registerComponent<Components::Velocity>("./plugins/bin/components/", "Velocity"))
        std::cerr << "Error: Could not register Velocity component in system Input" << std::endl;
    if (!engine.registerComponent<Components::Gun>("./plugins/bin/components/", "Gun"))
        std::cerr << "Error: Could not register Gun component in system Input" << std::endl;
    if (!engine.registerComponent<Components::Damage>("./plugins/bin/components/", "Damage"))
        std::cerr << "Error: Could not register Damage component in system Input" << std::endl;
    if (!engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position"))
        std::cerr << "Error: Could not register Position component in system Input" << std::endl;
    if (!engine.registerComponent<Components::Collider>("./plugins/bin/components/", "Collider"))
        std::cerr << "Error: Could not register Collider component in system Input" << std::endl;
    if (!engine.registerComponent<Components::SpriteID>("./plugins/bin/components/", "SpriteID"))
        std::cerr << "Error: Could not register SpriteID component in system Input" << std::endl;
    if (!engine.registerComponent<Components::Type>("./plugins/bin/components/", "Type"))
        std::cerr << "Error: Could not register Type component in system Input" << std::endl;
    if (!engine.registerComponent<Components::DeathRange>("./plugins/bin/components/", "DeathRange"))
        std::cerr << "Error: Could not register DeathRange component in system Input" << std::endl;
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryPoint()
{
    return new Systems::AttackSystem();
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryConfig(libconfig::Setting &config)
{
    return new Systems::AttackSystem(config);
}
