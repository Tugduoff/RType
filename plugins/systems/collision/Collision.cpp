/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Collision.cpp file
*/

#include "ECS/utilities/Zipper/IndexedZipper.hpp"
#include "GameEngine/GameEngine.hpp"
#include "Collision.hpp"
#include "components/position/Position.hpp"
#include "components/collider/Collider.hpp"
#include "components/type/Type.hpp"
#include "components/damage/Damage.hpp"
#include "components/health/Health.hpp"
#include "library_entrypoint.hpp"
#include <iostream>
#include <stdexcept>

Systems::Collision::Collision(libconfig::Setting &)
{
}

void Systems::Collision::run(Engine::GameEngine &engine)
{
    auto &reg = engine.getRegistry();

    try {
        checkPlayerProjectileToEnemyCollision(reg);
        checkEnemyProjectileToPlayerCollision(reg);
    } catch (std::runtime_error &e) {
        std::cerr << "Collision Error: " << e.what() << std::endl;
    }
}

void Systems::Collision::init(Engine::GameEngine &engine)
{
    engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position");
    engine.registerComponent<Components::Collider>("./plugins/bin/components/", "Collider");
    engine.registerComponent<Components::Type>("./plugins/bin/components/", "Type");
    engine.registerComponent<Components::Damage>("./plugins/bin/components/", "Damage");
    engine.registerComponent<Components::Health>("./plugins/bin/components/", "Health");
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryPoint()
{
    return new Systems::Collision();
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryConfig(libconfig::Setting &config)
{
    return new Systems::Collision(config);
}

void Systems::Collision::checkPlayerProjectileToEnemyCollision(ECS::Registry &reg)
{
    auto &typeArr = reg.componentManager().getComponents<Components::Type>();
    auto &posArr = reg.componentManager().getComponents<Components::Position>();
    auto &colliderArr = reg.componentManager().getComponents<Components::Collider>();
    auto &dmgArr = reg.componentManager().getComponents<Components::Damage>();
    auto &healthArr = reg.componentManager().getComponents<Components::Health>();

    std::vector<ECS::Entity> enemies;
    std::vector<ECS::Entity> projectiles;

    for (auto &&[i, type] : IndexedZipper(typeArr)) {
        if (type.id == Components::TypeID::ENEMY) {
            enemies.push_back((ECS::Entity)i);
        } else if (type.id == Components::TypeID::ALLY_PROJECTILE) {
            projectiles.push_back((ECS::Entity)i);
        }
    }
    for (auto &proj : projectiles) {
        for (auto &enemy : enemies) {
            try {
                auto &projPos = posArr[proj];
                auto &projCollider = colliderArr[proj];
                auto &enemyPos = posArr[enemy];
                auto &enemyCollider = colliderArr[enemy];

                if (projPos->x - projCollider->width / 2 < enemyPos->x + enemyCollider->width / 2 &&
                    projPos->x + projCollider->width / 2 > enemyPos->x - enemyCollider->width / 2 &&
                    projPos->y - projCollider->height / 2 < enemyPos->y + enemyCollider->height / 2 &&
                    projPos->y + projCollider->height / 2 > enemyPos->y - enemyCollider->height / 2) {
                    auto &projDamage = dmgArr[proj];
                    auto &enemyHealth = healthArr[enemy];

                    enemyHealth->currentHealth -= projDamage->damage;

                    std::cerr << "Projectile: " << proj << " hit enemy. Enemy health: " << enemyHealth->currentHealth << std::endl;
                    if (enemyHealth->currentHealth <= 0) {
                        reg.killEntity(enemy);
                    }
                    reg.killEntity(proj);
                }
            } catch (std::exception &e) {
                continue;
            }
        }
    }
}

void Systems::Collision::checkEnemyProjectileToPlayerCollision(ECS::Registry &reg)
{
    auto &typeArr = reg.componentManager().getComponents<Components::Type>();
    auto &posArr = reg.componentManager().getComponents<Components::Position>();
    auto &colliderArr = reg.componentManager().getComponents<Components::Collider>();
    auto &dmgArr = reg.componentManager().getComponents<Components::Damage>();
    auto &healthArr = reg.componentManager().getComponents<Components::Health>();

    std::vector<ECS::Entity> players;
    std::vector<ECS::Entity> projectiles;

    for (auto &&[i, type] : IndexedZipper(typeArr)) {
        if (type.id == Components::TypeID::ALLY) {
            players.push_back((ECS::Entity)i);
        } else if (type.id == Components::TypeID::ENEMY_PROJECTILE) {
            projectiles.push_back((ECS::Entity)i);
        }
    }
    for (auto &proj : projectiles) {
        for (auto &player : players) {
            try {
                auto &projPos = posArr[proj];
                auto &projCollider = colliderArr[proj];
                auto &playerPos = posArr[player];
                auto &playerCollider = colliderArr[player];

                if (projPos->x - projCollider->width / 2 < playerPos->x + playerCollider->width / 2 &&
                    projPos->x + projCollider->width / 2 > playerPos->x - playerCollider->width / 2 &&
                    projPos->y - projCollider->height / 2 < playerPos->y + playerCollider->height / 2 &&
                    projPos->y + projCollider->height / 2 > playerPos->y - playerCollider->height / 2) {
                    auto &projDamage = dmgArr[proj];
                    auto &playerHealth = healthArr[player];

                    playerHealth->currentHealth -= projDamage->damage;

                    std::cerr << "Projectile: " << proj << " hit player. Player health: " << playerHealth->currentHealth << std::endl;
                    if (playerHealth->currentHealth <= 0) {
                        reg.killEntity(player);
                    }
                    reg.killEntity(proj);
                }
            } catch (std::exception &e) {
                continue;
            }
        }
    }
}
