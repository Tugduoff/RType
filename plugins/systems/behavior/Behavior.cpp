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

                if (aiBehavior->behavior == 0)
                    continue;
                else if (aiBehavior->behavior == 1) {
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
                    if (pos->y < deathRange->minY + 50) {
                        vel->floatY = 3;
                        vel->floatY *= factor;
                        vel->y = (int)vel->floatY;
                    } else if (pos->y > deathRange->maxY - 800) {
                        vel->floatY = -3;
                        vel->floatY *= factor;
                        vel->y = (int)vel->floatY;
                    } else if (pos->y > deathRange->minY + 50 && pos->y < deathRange->maxY - 800 && vel->y != 3 && vel->y != -3) {
                        vel->floatY = 3;
                        vel->floatY *= factor;
                        vel->y = (int)vel->floatY;
                    }
                    engine.updateComponent(i, vel->getId(), vel->serialize());
                } else if (aiBehavior->behavior == 3) {
                    if (pos->y < deathRange->minY + 300) {
                        vel->floatY = 3;
                        vel->floatY *= factor;
                        vel->y = (int)vel->floatY;
                    } else if (pos->y > deathRange->maxY - 600) {
                        vel->floatY = -3;
                        vel->floatY *= factor;
                        vel->y = (int)vel->floatY;
                    } else if (pos->y > deathRange->minY + 300 && pos->y < deathRange->maxY - 600 && vel->y != 3 && vel->y != -3) {
                        vel->floatY = 3;
                        vel->floatY *= factor;
                        vel->y = (int)vel->floatY;
                    }
                    engine.updateComponent(i, vel->getId(), vel->serialize());
                } else if (aiBehavior->behavior == 4) {
                    if (pos->y < deathRange->minY + 500) {
                        vel->floatY = 3;
                        vel->floatY *= factor;
                        vel->y = (int)vel->floatY;
                    } else if (pos->y > deathRange->maxY - 250) {
                        vel->floatY = -3;
                        vel->floatY *= factor;
                        vel->y = (int)vel->floatY;
                    } else if (pos->y > deathRange->minY + 500 && pos->y < deathRange->maxY - 250 && vel->y != 3 && vel->y != -3) {
                        vel->floatY = 3;
                        vel->floatY *= factor;
                        vel->y = (int)vel->floatY;
                    }
                    engine.updateComponent(i, vel->getId(), vel->serialize());
                } else if (aiBehavior->behavior == 5) {
                    if (pos->y < deathRange->minY + 800) {
                        vel->floatY = 3;
                        vel->floatY *= factor;
                        vel->y = (int)vel->floatY;
                    } else if (pos->y > deathRange->maxY - 50) {
                        vel->floatY = -3;
                        vel->floatY *= factor;
                        vel->y = (int)vel->floatY;
                    } else if (pos->y > deathRange->minY + 800 && pos->y < deathRange->maxY - 50 && vel->y != 3 && vel->y != -3) {
                        vel->floatY = 3;
                        vel->floatY *= factor;
                        vel->y = (int)vel->floatY;
                    }
                    engine.updateComponent(i, vel->getId(), vel->serialize());
                } else if (aiBehavior->behavior == 6) {
                    vel->floatY = 0;
                    vel->y = 0;
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
                } else if (aiBehavior->behavior == 7) {
                    if (pos->x < deathRange->minX + 50) {
                        vel->floatX = 3;
                        vel->floatX *= factor;
                        vel->x = (int)vel->floatX;
                    } else if (pos->x > deathRange->maxX - 1400) {
                        vel->floatX = -3;
                        vel->floatX *= factor;
                        vel->y = (int)vel->floatX;
                    } else if (pos->x > deathRange->minX + 50 && pos->x < deathRange->maxX - 1440 && vel->x != 3 && vel->x != -3) {
                        vel->floatX = 3;
                        vel->floatX *= factor;
                        vel->x = (int)vel->floatX;
                    }
                    engine.updateComponent(i, vel->getId(), vel->serialize());
                } else if (aiBehavior->behavior == 8) {
                    if (pos->x < deathRange->minX + 450) {
                        vel->floatX = 3;
                        vel->floatX *= factor;
                        vel->y = (int)vel->floatX;
                    } else if (pos->x > deathRange->maxX - 1000) {
                        vel->floatX = -3;
                        vel->floatX *= factor;
                        vel->x = (int)vel->floatX;
                    } else if (pos->x > deathRange->minX + 450 && pos->x < deathRange->maxX - 1000 && vel->x != 3 && vel->x != -3) {
                        vel->floatX = 3;
                        vel->floatX *= factor;
                        vel->x = (int)vel->floatX;
                    }
                    engine.updateComponent(i, vel->getId(), vel->serialize());
                } else if (aiBehavior->behavior == 9) {
                    if (pos->x < deathRange->minX + 850) {
                        vel->floatX = 3;
                        vel->floatX *= factor;
                        vel->x = (int)vel->floatX;
                    } else if (pos->x > deathRange->maxX - 400) {
                        vel->floatX = -3;
                        vel->floatX *= factor;
                        vel->x = (int)vel->floatX;
                    } else if (pos->x > deathRange->minX + 50 && pos->x < deathRange->maxX - 800 && vel->x != 3 && vel->x != -3) {
                        vel->floatX = 3;
                        vel->floatX *= factor;
                        vel->x = (int)vel->floatX;
                    }
                    engine.updateComponent(i, vel->getId(), vel->serialize());
                } else if (aiBehavior->behavior == 10) {
                    if (pos->x < deathRange->minX + 1250) {
                        vel->floatX = 3;
                        vel->floatX *= factor;
                        vel->x = (int)vel->floatX;
                    } else if (pos->x > deathRange->maxX - 50) {
                        vel->floatX = -3;
                        vel->floatX *= factor;
                        vel->x = (int)vel->floatX;
                    } else if (pos->x > deathRange->minX + 1250 && pos->x < deathRange->maxX - 50 && vel->x != 3 && vel->x != -3) {
                        vel->floatX = 3;
                        vel->floatX *= factor;
                        vel->x = (int)vel->floatX;
                    }
                    engine.updateComponent(i, vel->getId(), vel->serialize());
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