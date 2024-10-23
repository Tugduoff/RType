/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Attack.cpp file
*/

#include "GameEngine/GameEngine.hpp"
#include "Attack.hpp"
#include "components/gun/Gun.hpp"
#include "components/type/Type.hpp"
#include "components/velocity/Velocity.hpp"
#include "components/position/Position.hpp"
#include "components/collider/Collider.hpp"
#include "components/damage/Damage.hpp"
#include "components/spriteId/SpriteID.hpp"
#include "components/deathRange/DeathRange.hpp"
#include "library_entrypoint.hpp"
#include "ECS/utilities/Zipper/Zipper.hpp"
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
        auto &gunArr = reg.componentManager().getComponents<Components::Gun>();
        auto &posArr = reg.componentManager().getComponents<Components::Position>();
        auto &typeArr = reg.componentManager().getComponents<Components::Type>();

        for (auto &&[pos, gun, id] : Zipper(posArr, gunArr, typeArr)) {
            if (gun.chrono.getElapsedTime() < gun.fireRate)
                continue;
            if (id.id == Components::TypeID::ALLY)
                continue;
            gun.chrono.restart();

            createProjectile(
                engine,
                pos.x,
                pos.y,
                gun.bulletVelocity,
                0,
                10,
                10,
                gun.bulletDamage,
                Components::TypeID::ENEMY_PROJECTILE,
                gun.spriteId
            );
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
