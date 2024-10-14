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
#include "components/spriteId/SpriteID.hpp"
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
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void Systems::Collision::init(Engine::GameEngine &engine)
{
    if (!engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position"))
        std::cerr << "Error: Could not register Position component in system Collision" << std::endl;
    if (!engine.registerComponent<Components::Collider>("./plugins/bin/components/", "Collider"))
        std::cerr << "Error: Could not register Collider component in system Collision" << std::endl;
    if (!engine.registerComponent<Components::SpriteIDComponent>("./plugins/bin/components/", "SpriteID"))
        std::cerr << "Error: Could not register SpriteID component in system Collision" << std::endl;
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
    auto &spriteIdComponents = reg.componentManager().getComponents<Components::SpriteIDComponent>();
    auto &posComponents = reg.componentManager().getComponents<Components::Position>();
    auto &colliderComponents = reg.componentManager().getComponents<Components::Collider>();
    auto &damageComponents = reg.componentManager().getComponents<Components::Damage>();
    auto &healthComponents = reg.componentManager().getComponents<Components::Health>();

    std::vector<ECS::Entity> enemies;
    std::vector<ECS::Entity> projectiles;

    size_t i = 0;
    for (i = 0; i < spriteIdComponents.size(); i++) {
        try {
            auto &spriteId = spriteIdComponents[i];

            if (spriteId->id == Components::SpriteID::Enemy) {
                enemies.push_back((ECS::Entity)i);
            } else if (spriteId->id == Components::SpriteID::ProjectileRight) {
                projectiles.push_back((ECS::Entity)i);
            }
        } catch (std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            continue;
        }
    }
    for (auto &proj : projectiles) {
        for (auto &enemy : enemies) {
            try {
                std::cerr << "Checking collision between projectile " << proj << " and enemy " << enemy << std::endl;
                auto &projPos = posComponents[proj];
                auto &projCollider = colliderComponents[proj];
                auto &enemyPos = posComponents[enemy];
                auto &enemyCollider = colliderComponents[enemy];
                std::cerr << "Values are for proj : " << projPos->x << " " << projPos->y << " " << projCollider->width << " " << projCollider->height << std::endl;
                std::cerr << "Values are for enemy : " << enemyPos->x << " " << enemyPos->y << " " << enemyCollider->width << " " << enemyCollider->height << std::endl;
                if (projPos->x - projCollider->width / 2 < enemyPos->x + enemyCollider->width / 2 &&
                    projPos->x + projCollider->width / 2 > enemyPos->x - enemyCollider->width / 2 &&
                    projPos->y - projCollider->height / 2 < enemyPos->y + enemyCollider->height / 2 &&
                    projPos->y + projCollider->height / 2 > enemyPos->y - enemyCollider->height / 2) {
                    auto &projDamage = damageComponents[proj];
                    auto &enemyHealth = healthComponents[enemy];

                    enemyHealth->currentHealth -= projDamage->damage;

                    std::cerr << "Projectile hit enemy. Enemy health: " << enemyHealth->currentHealth << std::endl;
                    if (enemyHealth->currentHealth <= 0) {
                        reg.killEntity(enemy);
                    }
                    reg.killEntity(proj);
                }
            } catch (std::exception &e) {
                std::cerr << "Collision Error: " << e.what() << std::endl;
                continue;
            }
        }
    }
}

void Systems::Collision::checkEnemyProjectileToPlayerCollision(ECS::Registry &reg) {
    auto &spriteIdComponents = reg.componentManager().getComponents<Components::SpriteIDComponent>();
    auto &posComponents = reg.componentManager().getComponents<Components::Position>();
    auto &colliderComponents = reg.componentManager().getComponents<Components::Collider>();
    auto &damageComponents = reg.componentManager().getComponents<Components::Damage>();
    auto &healthComponents = reg.componentManager().getComponents<Components::Health>();

    std::vector<ECS::Entity> players;
    std::vector<ECS::Entity> projectiles;

    size_t i = 0;
    for (i = 0; i < spriteIdComponents.size(); i++) {
        try {
            auto &spriteId = spriteIdComponents[i];

            if (spriteId->id == Components::SpriteID::Player) {
                players.push_back((ECS::Entity)i);
            } else if (spriteId->id == Components::SpriteID::ProjectileLeft) {
                projectiles.push_back((ECS::Entity)i);
            }
        } catch (std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            continue;
        }
    }
    for (auto &proj : projectiles) {
        for (auto &player : players) {
            try {
                std::cerr << "Checking collision between projectile " << proj << " and player " << player << std::endl;
                auto &projPos = posComponents[proj];
                auto &projCollider = colliderComponents[proj];
                auto &playerPos = posComponents[player];
                auto &playerCollider = colliderComponents[player];
                std::cerr << "Values are for proj : " << projPos->x << " " << projPos->y << " " << projCollider->width << " " << projCollider->height << std::endl;
                std::cerr << "Values are for player : " << playerPos->x << " " << playerPos->y << " " << playerCollider->width << " " << playerCollider->height << std::endl;
                if (projPos->x - projCollider->width / 2 < playerPos->x + playerCollider->width / 2 &&
                    projPos->x + projCollider->width / 2 > playerPos->x - playerCollider->width / 2 &&
                    projPos->y - projCollider->height / 2 < playerPos->y + playerCollider->height / 2 &&
                    projPos->y + projCollider->height / 2 > playerPos->y - playerCollider->height / 2) {
                    auto &projDamage = damageComponents[proj];
                    auto &playerHealth = healthComponents[player];

                    playerHealth->currentHealth -= projDamage->damage;

                    std::cerr << "Projectile hit player. Player health: " << playerHealth->currentHealth << std::endl;
                    if (playerHealth->currentHealth <= 0) {
                        reg.killEntity(player);
                    }
                    reg.killEntity(proj);
                }
            } catch (std::exception &e) {
                std::cerr << "Collision Error: " << e.what() << std::endl;
                continue;
            }
        }
    }
}
