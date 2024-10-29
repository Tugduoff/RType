/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Ai.cpp file
*/

#include "GameEngine/GameEngine.hpp"
#include "Behavior.hpp"
#include "components/gun/Gun.hpp"
#include "library_entrypoint.hpp"
#include <iostream>
#include <stdexcept>

Systems::BehaviorSystem::BehaviorSystem(libconfig::Setting &)
{
}

void Systems::BehaviorSystem::run(Engine::GameEngine &engine)
{
    auto &reg = engine.getRegistry();
    
    try {
        auto &aiComponent = reg.componentManager().getComponents<Components::Ai>();
        auto &posComponent = reg.componentManager().getComponents<Components::Position>();
        auto &velComponent = reg.componentManager().getComponents<Components::Velocity>();
        auto &deathRangeComponent = reg.componentManager().getComponents<Components::DeathRange>();

        size_t i = 0;
        for (i = 0; i < aiComponent.size() || i < posComponent.size() || i < velComponent.size() || i < deathRangeComponent.size(); i++) {
            try {
                auto &aiBehavior = aiComponent[i];
                auto &pos = posComponent[i];
                auto &vel = velComponent[i];
                auto &deathRange = deathRangeComponent[i];
                float factor = (float)vel->diminishingFactor / 100;

                if (aiBehavior->startingX && aiBehavior->startingY) {
                    aiBehavior->startingX = pos->x;
                    aiBehavior->startingY = pos->y;
                }

                if (aiBehavior->behavior == 0)
                    continue;
                else if (aiBehavior->behavior == 1) {
                    std::cerr << "Entity " << i << " has behavior 1" << std::endl;
                    vel->floatX = 0;
                    vel->x = 0;
                    if (pos->y < deathRange->minY + 50) {
                        vel->floatY = 5;
                        vel->floatY *= factor;
                        vel->y = (int)vel->floatY;
                    } else if (pos->y > deathRange->maxY - 50) {
                        vel->floatY = -5;
                        vel->floatY *= factor;
                        vel->y = (int)vel->floatY;
                    } else if (pos->y > deathRange->minY + 50 && pos->y < deathRange->maxY - 50 && vel->y != 5 && vel->y != -5) {
                        vel->floatY = 5;
                        vel->floatY *= factor;
                        vel->y = (int)vel->floatY;
                    }
                    engine.updateComponent(i, vel->getId(), vel->serialize());
                } else if (aiBehavior->behavior == 2) {
                    std::cerr << "Entity " << i << " has behavior 2" << std::endl;
                    vel->x = -2;
                    if (pos->y > aiBehavior->startingY + 100) {
                        vel->y = -5;
                    } else if (pos->y < aiBehavior->startingY - 100) {
                        vel->y = 5;
                    }
                    engine.updateComponent(i, vel->getId(), vel->serialize());
                } else if (aiBehavior->behavior == 3) {
                    std::cerr << "Entity " << i << " has behavior 3" << std::endl;
                } else if (aiBehavior->behavior == 4) {
                    std::cerr << "Entity " << i << " has behavior 4" << std::endl;
                }
            } catch (std::exception &e) {
                continue;
            }
        }
    } catch (std::exception &e) {
        std::cerr << "IA Error: " << e.what() << std::endl;
    }
}

void Systems::BehaviorSystem::init(Engine::GameEngine &engine)
{
    if (!engine.registerComponent<Components::Ai>("./plugins/bin/components/", "Ai"))
        std::cerr << "Error: Could not register Ai component in Ai system" << std::endl;
    if (!engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position"))
        std::cerr << "Error: Could not register Position component in Ai system" << std::endl;
    if (!engine.registerComponent<Components::Velocity>("./plugins/bin/components/", "Velocity"))
        std::cerr << "Error: Could not register Velocity component in Ai system" << std::endl;
    if (!engine.registerComponent<Components::DeathRange>("./plugins/bin/components/", "DeathRange"))
        std::cerr << "Error: Could not register DeathRange component in Ai system" << std::endl;
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryPoint()
{
    return new Systems::BehaviorSystem();
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryConfig(libconfig::Setting &config)
{
    return new Systems::BehaviorSystem(config);
}
