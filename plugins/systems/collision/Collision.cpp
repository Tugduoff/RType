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
#include "components/sound/Sound.hpp"
#include "components/destruction/Destruction.hpp"
#include "library_entrypoint.hpp"
#include <iostream>
#include <stdexcept>

Systems::Collision::Collision(libconfig::Setting &) : __deathEffectsCooldown()
{
}

void Systems::Collision::run(Engine::GameEngine &engine)
{
    try {
        checkChronos(engine);
        checkPlayerProjectileToEnemyCollision(engine);
        checkEnemyProjectileToPlayerCollision(engine);
    } catch (std::runtime_error &e) {
        std::cerr << "Collision Error: " << e.what() << std::endl;
    }
}

void Systems::Collision::checkChronos(Engine::GameEngine &engine)
{
    for (auto deathEffectCooldown : __deathEffectsCooldown) {
        if (std::get<1>(deathEffectCooldown).getElapsedTime() >= std::get<2>(deathEffectCooldown)) {
            size_t id = std::get<0>(deathEffectCooldown);
            engine.getRegistry().killEntity((ECS::Entity)id);
            // Delete element from the vector where the entity is the first element
            std::erase_if(__deathEffectsCooldown, [id](const std::tuple<size_t, Chrono, uint32_t> &elem) {
                return std::get<0>(elem) == id;
            });
        }
    }
}

void Systems::Collision::init(Engine::GameEngine &engine)
{
    engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position");
    engine.registerComponent<Components::Collider>("./plugins/bin/components/", "Collider");
    engine.registerComponent<Components::Type>("./plugins/bin/components/", "Type");
    engine.registerComponent<Components::Damage>("./plugins/bin/components/", "Damage");
    engine.registerComponent<Components::Health>("./plugins/bin/components/", "Health");
    engine.registerComponent<Components::Destruction>("./plugins/bin/components/", "Destruction");
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

void Systems::Collision::checkPlayerProjectileToEnemyCollision(Engine::GameEngine &engine)
{
    auto &reg = engine.getRegistry();

    auto &typeArr = reg.componentManager().getComponents<Components::Type>();
    auto &posArr = reg.componentManager().getComponents<Components::Position>();
    auto &colliderArr = reg.componentManager().getComponents<Components::Collider>();
    auto &dmgArr = reg.componentManager().getComponents<Components::Damage>();
    auto &healthArr = reg.componentManager().getComponents<Components::Health>();
    auto &soundArr = reg.componentManager().getComponents<Components::Sound>();

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

                    try {
                        if (enemyHealth->currentHealth <= 0) {
                            spawnDeathEffect(engine, enemy);
                            reg.killEntity(enemy);
                            reg.killEntity(proj);
                            continue;
                        }
                        auto &sound = soundArr[enemy];

                        for (auto &soundInstance : sound->sounds) {
                            if (std::get<0>(soundInstance) == "HIT") {
                                if (std::get<5>(soundInstance) == true) {
                                    std::get<5>(soundInstance) = false;
                                    engine.updateComponent((ECS::Entity)enemy, sound->getId(), sound->serialize());
                                    enemyHealth->currentHealth += projDamage->damage;
                                    return;
                                }
                                std::get<5>(soundInstance) = true;
                                engine.updateComponent((ECS::Entity)enemy, sound->getId(), sound->serialize());
                            }
                        }
                    } catch (std::exception &) {}

                    if (enemyHealth->currentHealth <= 0) {
                        spawnDeathEffect(engine, enemy);
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

void Systems::Collision::checkEnemyProjectileToPlayerCollision(Engine::GameEngine &engine)
{
    auto &reg = engine.getRegistry();

    auto &typeArr = reg.componentManager().getComponents<Components::Type>();
    auto &posArr = reg.componentManager().getComponents<Components::Position>();
    auto &colliderArr = reg.componentManager().getComponents<Components::Collider>();
    auto &dmgArr = reg.componentManager().getComponents<Components::Damage>();
    auto &healthArr = reg.componentManager().getComponents<Components::Health>();
    auto &soundArr = reg.componentManager().getComponents<Components::Sound>();

    std::vector<ECS::Entity> players;
    std::vector<ECS::Entity> projectiles;

    for (auto &&[i, type] : IndexedZipper(typeArr)) {
        if (type.id == Components::TypeID::ALLY) {
            players.push_back((ECS::Entity)i);
        } else if (type.id == Components::TypeID::ENEMY_PROJECTILE) {
            projectiles.push_back((ECS::Entity)i);
        }
    }

    // for (auto &player : players) {
    //     std::cerr << "Player: " << player << std::endl;
    // }

    // for (auto &proj : projectiles) {
    //     std::cerr << "Projectile: " << proj << std::endl;
    // }

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

                    try {
                        if (playerHealth->currentHealth <= 0) {
                            spawnDeathEffect(engine, player);
                            reg.killEntity(player);
                            reg.killEntity(proj);
                            continue;
                        }
                        auto &sound = soundArr[player];

                        for (auto &soundInstance : sound->sounds) {
                            if (std::get<0>(soundInstance) == "HIT") {
                                if (std::get<5>(soundInstance) == true) {
                                    std::get<5>(soundInstance) = false;
                                    engine.updateComponent((ECS::Entity)player, sound->getId(), sound->serialize());
                                    playerHealth->currentHealth += projDamage->damage;
                                    return;
                                }
                                std::get<5>(soundInstance) = true;
                                engine.updateComponent((ECS::Entity)player, sound->getId(), sound->serialize());
                            }
                        }
                    } catch (std::exception &) {}

                    if (playerHealth->currentHealth <= 0) {
                        spawnDeathEffect(engine, player);
                        reg.killEntity(player);
                    }
                    reg.killEntity(proj);
                }
            } catch (std::exception &e) {
                std::cerr << "Error checking enemy projectile to player collision: " << e.what() << std::endl;
                continue;
            }
        }
    }
}

void Systems::Collision::spawnDeathEffect(Engine::GameEngine &engine, ECS::Entity entity)
{
    auto &reg = engine.getRegistry();

    auto &posArr = reg.componentManager().getComponents<Components::Position>();
    auto &destroyArr = reg.componentManager().getComponents<Components::Destruction>();

    try {
        auto &pos = posArr[entity];
        auto &destroy = destroyArr[entity];

        ECS::Entity newEntity = engine.getRegistry().createEntity();
        std::cerr << "New entity created with ID: " << newEntity << std::endl;

        std::vector<std::any> posArgs = {
            static_cast<uint32_t>(pos->x),
            static_cast<uint32_t>(pos->y),
            static_cast<uint32_t>(1) 
        };

        std::unique_ptr<Components::IComponent> &posComp = engine.getComponentFromId("Position");
        posComp->addTo(newEntity, engine, posArgs);
        std::cerr << "Created and Added position to death effect" << std::endl;

        std::unique_ptr<Components::IComponent> &spriteIdComp = engine.getComponentFromId("SpriteID");
        spriteIdComp->addTo(newEntity, engine, { destroy->id });
        std::cerr << "Created and Added sprite ID to death effect" << std::endl;

        std::vector<std::tuple<std::string, std::string, uint8_t, uint8_t, bool, bool>> sounds = {
            std::make_tuple("AMBIENT",
                std::get<0>(destroy->sound),
                std::get<1>(destroy->sound),
                std::get<2>(destroy->sound),
                std::get<3>(destroy->sound), true)
        };
        std::unique_ptr<Components::IComponent> &soundComp = engine.getComponentFromId("Sound");
        soundComp->addTo(newEntity, engine, { sounds });
        std::cerr << "Created and Added sound to death effect" << std::endl;

        std::unique_ptr<Components::IComponent> &scaleComp = engine.getComponentFromId("Scale");
        scaleComp->addTo(newEntity, engine, {
            std::get<0>(destroy->scale),
            std::get<1>(destroy->scale)
        });
        std::cerr << "Created and Added scale to death effect" << std::endl;

        // starting chrono
        Chrono newChrono;

        __deathEffectsCooldown.push_back({ newEntity, newChrono, destroy->duration });
    } catch (std::exception &e) {
        std::cerr << "Error spawning death effect: " << e.what() << std::endl;
    }
}
