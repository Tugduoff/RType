/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Collision.cpp file
*/

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
    if (!engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position"))
        std::cerr << "Error: Could not register Position component in system Collision" << std::endl;
    if (!engine.registerComponent<Components::Collider>("./plugins/bin/components/", "Collider"))
        std::cerr << "Error: Could not register Collider component in system Collision" << std::endl;
    if (!engine.registerComponent<Components::Type>("./plugins/bin/components/", "Type"))
        std::cerr << "Error: Could not register Type component in system Collision" << std::endl;
    if (!engine.registerComponent<Components::Damage>("./plugins/bin/components/", "Damage"))
        std::cerr << "Error: Could not register Damage component in system Collision" << std::endl;
    if (!engine.registerComponent<Components::Health>("./plugins/bin/components/", "Health"))
        std::cerr << "Error: Could not register Health component in system Collision" << std::endl;
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

void Systems::Collision::checkPlayerProjectileToEnemyCollision(ECS::Registry &reg) {
    auto &typeComponents = reg.componentManager().getComponents<Components::Type>();
    auto &posComponents = reg.componentManager().getComponents<Components::Position>();
    auto &colliderComponents = reg.componentManager().getComponents<Components::Collider>();
    auto &damageComponents = reg.componentManager().getComponents<Components::Damage>();
    auto &healthComponents = reg.componentManager().getComponents<Components::Health>();

    std::vector<ECS::Entity> enemies;
    std::vector<ECS::Entity> projectiles;

    size_t i = 0;
    for (i = 0; i < typeComponents.size(); i++) {
        try {
            auto &type = typeComponents[i];

            if (type->id == Components::TypeID::ENEMY) {
                enemies.push_back((ECS::Entity)i);
            } else if (type->id == Components::TypeID::ALLY_PROJECTILE) {
                projectiles.push_back((ECS::Entity)i);
            }
        } catch (std::exception &e) {
            continue;
        }
    }
    for (auto &proj : projectiles) {
        for (auto &enemy : enemies) {
            try {
                auto &projPos = posComponents[proj];
                auto &projCollider = colliderComponents[proj];
                auto &enemyPos = posComponents[enemy];
                auto &enemyCollider = colliderComponents[enemy];

                if (projPos->x - projCollider->width / 2 < enemyPos->x + enemyCollider->width / 2 &&
                    projPos->x + projCollider->width / 2 > enemyPos->x - enemyCollider->width / 2 &&
                    projPos->y - projCollider->height / 2 < enemyPos->y + enemyCollider->height / 2 &&
                    projPos->y + projCollider->height / 2 > enemyPos->y - enemyCollider->height / 2) {
                    auto &projDamage = damageComponents[proj];
                    auto &enemyHealth = healthComponents[enemy];

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

void Systems::Collision::checkEnemyProjectileToPlayerCollision(ECS::Registry &reg) {
    auto &typeComponents = reg.componentManager().getComponents<Components::Type>();
    auto &posComponents = reg.componentManager().getComponents<Components::Position>();
    auto &colliderComponents = reg.componentManager().getComponents<Components::Collider>();
    auto &damageComponents = reg.componentManager().getComponents<Components::Damage>();
    auto &healthComponents = reg.componentManager().getComponents<Components::Health>();

    std::vector<ECS::Entity> players;
    std::vector<ECS::Entity> projectiles;

    size_t i = 0;
    for (i = 0; i < typeComponents.size(); i++) {
        try {
            auto &type = typeComponents[i];

            if (type->id == Components::TypeID::ALLY) {
                players.push_back((ECS::Entity)i);
            } else if (type->id == Components::TypeID::ENEMY_PROJECTILE) {
                projectiles.push_back((ECS::Entity)i);
            }
        } catch (std::exception &e) {
            continue;
        }
    }
    for (auto &proj : projectiles) {
        for (auto &player : players) {
            try {
                auto &projPos = posComponents[proj];
                auto &projCollider = colliderComponents[proj];
                auto &playerPos = posComponents[player];
                auto &playerCollider = colliderComponents[player];

                if (projPos->x - projCollider->width / 2 < playerPos->x + playerCollider->width / 2 &&
                    projPos->x + projCollider->width / 2 > playerPos->x - playerCollider->width / 2 &&
                    projPos->y - projCollider->height / 2 < playerPos->y + playerCollider->height / 2 &&
                    projPos->y + projCollider->height / 2 > playerPos->y - playerCollider->height / 2) {
                    auto &projDamage = damageComponents[proj];
                    auto &playerHealth = healthComponents[player];

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
