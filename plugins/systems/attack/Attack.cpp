/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Attack.cpp file
*/

#include "GameEngine/GameEngine.hpp"
#include "Attack.hpp"
#include "components/gun/Gun.hpp"
#include "utils/EntityActions.hpp"
#include "components/type/Type.hpp"
#include "components/velocity/Velocity.hpp"
#include "components/position/Position.hpp"
#include "components/collider/Collider.hpp"
#include "components/damage/Damage.hpp"
#include "components/spriteId/SpriteID.hpp"
#include "components/deathRange/DeathRange.hpp"
#include "components/action/Action.hpp"
#include "components/position/Position.hpp"
#include "components/type/Type.hpp"
#include "library_entrypoint.hpp"
#include "ECS/utilities/Zipper/IndexedZipper.hpp"
#include "utils/Projectile.hpp"
#include <iostream>
#include <stdexcept>
#include <tuple>

Systems::AttackSystem::AttackSystem(libconfig::Setting &)
{
}

void Systems::AttackSystem::run(Engine::GameEngine &engine)
{
    auto &reg = engine.getRegistry();

    try {
        auto &gunArr = reg.componentManager().getComponents<Components::Gun>();
        auto &posArr = reg.componentManager().getComponents<Components::Position>();
        auto &typeArr = reg.componentManager().getComponents<Components::Type>();
        auto &actArr = reg.componentManager().getComponents<Components::ActionComponent>();

        std::vector<std::tuple<
            decltype(Components::Position::x),
            decltype(Components::Position::y),
            decltype(Components::Gun::bulletVelocity),
            decltype(Components::Gun::bulletDamage),
            Components::TypeID,
            decltype(Components::Gun::spriteId),
            decltype(Components::Gun::soundPath),
            decltype(Components::Gun::volume),
            decltype(Components::Gun::pitch)
        >> projToCreate;

        for (auto &&[i, pos, gun, id] : IndexedZipper(posArr, gunArr, typeArr)) {
            if (gun.chrono.getElapsedTime() < gun.fireRate)
                continue;
            if (id.id == Components::TypeID::ALLY)
                continue;
            std::cerr << "Entity: " << i << " is attacking" << std::endl;
            try {
                auto &entityAction = actArr[i];

                if (entityAction->action == EntityAction::SHOOT_FORWARD && gun.chrono.getElapsedTime() > gun.fireRate / 2) {
                    entityAction->action = EntityAction::IDLE;
                    engine.updateComponent((ECS::Entity)i, entityAction->getId(), entityAction->serialize());
                }
            } catch (std::exception &e) {
                std::unique_ptr<Components::ActionComponent> actionComp = std::make_unique<Components::ActionComponent>();
                engine.getRegistry().componentManager().addComponent<Components::ActionComponent>((ECS::Entity)i, std::move(actionComp));
                std::cerr << "Set default action for entity: " << i << " in ActionManager." << std::endl;
                continue;
            }

            gun.chrono.restart();

            projToCreate.emplace_back(
                pos.x,
                pos.y,
                gun.bulletVelocity,
                gun.bulletDamage,
                Components::TypeID::ENEMY_PROJECTILE,
                gun.spriteId,
                gun.soundPath,
                gun.volume,
                gun.pitch
            );

            std::cerr << "Entity: " << i << " fired a shot" << std::endl;
        }
        bool loop = false;

        for (const auto &[posX, posY, bulletVel, bulletDmg, typeId, spriteId, soundPath, volume, pitch] : projToCreate) {
            createProjectile(
                engine,
                posX, posY,
                bulletVel, 0, 10, 10, bulletDmg,
                typeId, spriteId,
                soundPath, volume, pitch, loop);
        }
    } catch (std::runtime_error &e) {
        std::cerr << "Attack Error: " << e.what() << std::endl;
    }
}

void Systems::AttackSystem::init(Engine::GameEngine &engine)
{
    engine.registerComponent<Components::Velocity>("./plugins/bin/components/", "Velocity");
    engine.registerComponent<Components::Gun>("./plugins/bin/components/", "Gun");
    engine.registerComponent<Components::Damage>("./plugins/bin/components/", "Damage");
    engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position");
    engine.registerComponent<Components::Collider>("./plugins/bin/components/", "Collider");
    engine.registerComponent<Components::SpriteID>("./plugins/bin/components/", "SpriteID");
    engine.registerComponent<Components::Type>("./plugins/bin/components/", "Type");
    engine.registerComponent<Components::DeathRange>("./plugins/bin/components/", "DeathRange");
    engine.registerComponent<Components::ActionComponent>("./plugins/bin/components/", "Action");
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
