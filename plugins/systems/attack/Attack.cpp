/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Attack.cpp file
*/

#include "GameEngine/GameEngine.hpp"
#include "Attack.hpp"
#include "components/gun/Gun.hpp"
#include "library_entrypoint.hpp"
#include "ECS/utilities/Zipper/Zipper.hpp"
#include <iostream>
#include <stdexcept>

Systems::AttackSystem::AttackSystem(libconfig::Setting &)
{
}

void Systems::AttackSystem::createProjectile(Engine::GameEngine &engine, 
    int posX, int posY, 
    int velX, int velY, 
    int colliderWidth,
    int colliderHeight, 
    int damageValue,
    enum Components::TypeID type,
    const std::string &spriteID
)
{
    auto &reg = engine.getRegistry();
    ECS::Entity projectile = reg.createEntity();

    std::cerr << "Created new projectile with id: " << projectile << std::endl;

    std::unique_ptr<Components::Position> positionComponent =
        std::make_unique<Components::Position>(posX, posY, 1);
    reg.componentManager().addComponent(projectile, std::move(positionComponent));
    std::unique_ptr<Components::Velocity> velocityComponent =
        std::make_unique<Components::Velocity>(velX, velY, 100);
    reg.componentManager().addComponent(projectile, std::move(velocityComponent));
    std::unique_ptr<Components::Collider> colliderComponent =
        std::make_unique<Components::Collider>(colliderWidth, colliderHeight);
    reg.componentManager().addComponent(projectile, std::move(colliderComponent));
    std::unique_ptr<Components::Damage> damageComponent =
        std::make_unique<Components::Damage>(damageValue);
    reg.componentManager().addComponent(projectile, std::move(damageComponent));
    std::unique_ptr<Components::Type> typeComponent =
        std::make_unique<Components::Type>(type);
    reg.componentManager().addComponent(projectile, std::move(typeComponent));
    std::unique_ptr<Components::SpriteID> spriteIDComponent =
        std::make_unique<Components::SpriteID>(spriteID);
    reg.componentManager().addComponent(projectile, std::move(spriteIDComponent));
    std::unique_ptr<Components::DeathRange> deathRangeComponent =
        std::make_unique<Components::DeathRange>(1920, 1080, 0, 0);
    reg.componentManager().addComponent(projectile, std::move(deathRangeComponent));
    return;
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

            int projectilePosX = pos.x;
            int projectilePosY = pos.y;
            int projectileVelX = gun.bulletVelocity;
            int projectileVelY = 0;
            int projectileColliderWidth = 10;
            int projectileColliderHeight = 10;
            int projectileDamage = gun.bulletDamage;
            enum Components::TypeID type = Components::TypeID::ENEMY_PROJECTILE;
            std::string spriteID = gun.spriteId;

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
