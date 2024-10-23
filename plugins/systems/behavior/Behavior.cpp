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
        try {
            for (i = 0; i < aiComponent.size() || i < posComponent.size() || i < velComponent.size() || i < deathRangeComponent.size(); i++) {
                auto &aiBehavior = aiComponent[i];
                auto &posBehavior = posComponent[i];
                auto &velBehavior = velComponent[i];
                auto &deathRangeBehavior = deathRangeComponent[i];

                if (aiBehavior->behavior == 0)
                    continue;
                if (aiBehavior->behavior == 1) {
                    std::cerr << "Entity " << i << " has behavior 1" << std::endl;
                    velBehavior->floatX = 0;
                    if (posBehavior->y > deathRangeBehavior->minY + 50)
                        velBehavior->floatY = 5;
                    else if (posBehavior->y < deathRangeBehavior->maxY - 50)
                        velBehavior->floatY = -5;
                    engine.updateComponent(i, velBehavior->getId(), velBehavior->serialize());
                }
                if (aiBehavior->behavior == 2) {
                    std::cerr << "Entity " << i << " has behavior 2" << std::endl;
                }
            }
        } catch (std::exception &e) {
            std::cerr << "IA for loop Error: " << e.what() << std::endl;
        }
    } catch (std::exception &e) {
        std::cerr << " IA Error: " << e.what() << std::endl;
    }
}

void Systems::BehaviorSystem::init(Engine::GameEngine &engine)
{
    if (!engine.registerComponent<Components::Ai>("./plugins/bin/components/", "Ai"))
        std::cerr << "Error: Could not register Ai component in system Ai" << std::endl;
    if (!engine.registerComponent<Components::Position>("./plugins/bin/components/", "Postion"))
        std::cerr << "Error: Could not register Position component in system Ai" << std::endl;
    if (!engine.registerComponent<Components::Velocity>("./plugins/bin/components/", "Velocity"))
        std::cerr << "Error: Could not register Velocity component in system Ai" << std::endl;
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
