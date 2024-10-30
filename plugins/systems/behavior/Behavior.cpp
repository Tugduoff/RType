/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Ai.cpp file
*/

#include "GameEngine/GameEngine.hpp"
#include "Behavior.hpp"
#include "library_entrypoint.hpp"
#include <iostream>
#include <stdexcept>

void Systems::BehaviorSystem::yAxisLoop(Engine::GameEngine &engine, size_t i, std::unique_ptr<Components::Position> &pos,
    std::unique_ptr<Components::Velocity> &vel, std::unique_ptr<Components::DeathRange> &deathRange, float factor)
{
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
    return;
}

void Systems::BehaviorSystem::yZigZag(Engine::GameEngine &engine, size_t i, std::unique_ptr<Components::Position> &pos,
    std::unique_ptr<Components::Velocity> &vel, std::unique_ptr<Components::DeathRange> &deathRange, float factor, 
    int minValue, int maxValue)
{
    if (pos->y < deathRange->minY + minValue) {
        vel->floatY = 3;
        vel->floatY *= factor;
        vel->y = (int)vel->floatY;
    } else if (pos->y > deathRange->maxY - maxValue) {
        vel->floatY = -3;
        vel->floatY *= factor;
        vel->y = (int)vel->floatY;
    } else if (pos->y > deathRange->minY + minValue && pos->y < deathRange->maxY - maxValue && vel->y != 3 && vel->y != -3) {
        vel->floatY = 3;
        vel->floatY *= factor;
        vel->y = (int)vel->floatY;
    }
    engine.updateComponent(i, vel->getId(), vel->serialize());
    return;
}

void Systems::BehaviorSystem::xAxisLoop(Engine::GameEngine &engine, size_t i, std::unique_ptr<Components::Position> &pos,
    std::unique_ptr<Components::Velocity> &vel, std::unique_ptr<Components::DeathRange> &deathRange,
    std::unique_ptr<Components::Gun> &gun, float factor)
{
    vel->floatY = 0;
    vel->y = 0;
    if (gun->bulletVelocityY == 0) {
        gun->bulletVelocityY = 8;
        engine.updateComponent(i, gun->getId(), gun->serialize());
    }
    if (pos->x < deathRange->minX + 50) {
        vel->floatX = 5;
        vel->floatX *= factor;
        vel->x = (int)vel->floatX;
    } else if (pos->x > deathRange->maxX - 50) {
        vel->floatX = -5;
        vel->floatX *= factor;
        vel->x = (int)vel->floatX;
    } else if (pos->x > deathRange->minX + 50 && pos->x < deathRange->maxX - 50 && vel->x != 5 && vel->x != -5) {
        vel->floatX = 5;
        vel->floatX *= factor;
        vel->x = (int)vel->floatX;
    }
    engine.updateComponent(i, vel->getId(), vel->serialize());
    return;
}

void Systems::BehaviorSystem::xZigZag(Engine::GameEngine &engine, size_t i, std::unique_ptr<Components::Position> &pos,
    std::unique_ptr<Components::Velocity> &vel, std::unique_ptr<Components::DeathRange> &deathRange,
    std::unique_ptr<Components::Gun> &gun, float factor, int minValue, int maxValue)
{
    if (gun->bulletVelocityY == 0) {
        gun->bulletVelocityY = 8;
        engine.updateComponent(i, gun->getId(), gun->serialize());
    }
    if (pos->x < deathRange->minX + minValue) {
        vel->floatX = 3;
        vel->floatX *= factor;
        vel->x = (int)vel->floatX;
    } else if (pos->x > deathRange->maxX - maxValue) {
        vel->floatX = -3;
        vel->floatX *= factor;
        vel->y = (int)vel->floatX;
    } else if (pos->x > deathRange->minX + minValue && pos->x < deathRange->maxX - maxValue && vel->x != 3 && vel->x != -3) {
        vel->floatX = 3;
        vel->floatX *= factor;
        vel->x = (int)vel->floatX;
    }
    engine.updateComponent(i, vel->getId(), vel->serialize());
    return;
}

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
        auto &GunComponent = reg.componentManager().getComponents<Components::Gun>();

        size_t i = 0;
        for (i = 0; i < aiComponent.size() || i < posComponent.size() ||
            i < velComponent.size() || i < deathRangeComponent.size() ||
            i < GunComponent.size(); i++) {
            try {
                auto &aiBehavior = aiComponent[i];
                auto &pos = posComponent[i];
                auto &vel = velComponent[i];
                auto &deathRange = deathRangeComponent[i];
                auto &gun = GunComponent[i];
                float factor = (float)vel->diminishingFactor / 100;

                switch (aiBehavior->behavior) {
                    case Components::BehaviorId::NOTHING:
                        continue;
                    case Components::BehaviorId::Y_AXIS_LOOP:
                        yAxisLoop(engine, i, pos, vel, deathRange, factor);
                        break;
                    case Components::BehaviorId::Y_ZIG_ZAG_1:
                        yZigZag(engine, i, pos, vel, deathRange, factor, 50, 800);
                        break;
                    case Components::BehaviorId::Y_ZIG_ZAG_2:
                        yZigZag(engine, i, pos, vel, deathRange, factor, 300, 600);
                        break;
                    case Components::BehaviorId::Y_ZIG_ZAG_3:
                        yZigZag(engine, i, pos, vel, deathRange, factor, 500, 250);
                        break;
                    case Components::BehaviorId::Y_ZIG_ZAG_4:
                        yZigZag(engine, i, pos, vel, deathRange, factor, 800, 50);
                        break;
                    case Components::BehaviorId::X_AXIS_LOOP:
                        xAxisLoop(engine, i, pos, vel, deathRange, gun, factor);
                        break;
                    case Components::BehaviorId::X_ZIG_ZAG_1:
                        xZigZag(engine, i, pos, vel, deathRange, gun, factor, 50, 1400);
                        break;
                    case Components::BehaviorId::X_ZIG_ZAG_2:
                        xZigZag(engine, i, pos, vel, deathRange, gun, factor, 450, 1000);
                        break;
                    case Components::BehaviorId::X_ZIG_ZAG_3:
                        xZigZag(engine, i, pos, vel, deathRange, gun, factor, 800, 400);
                        break;
                    case Components::BehaviorId::X_ZIG_ZAG_4:
                        xZigZag(engine, i, pos, vel, deathRange, gun, factor, 1350, 50);
                        break;
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
    if (!engine.registerComponent<Components::Gun>("./plugins/bin/components/", "Gun"))
        std::cerr << "Error: Could not register Gun component in Ai system" << std::endl;
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
