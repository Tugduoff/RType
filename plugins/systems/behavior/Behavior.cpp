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
        size_t i = 0;
        try {
            for (i = 0; i < aiComponent.size(); i++) {
                auto &aiBehavior = aiComponent[i];
                if (aiBehavior->behavior == 0)
                    continue;
                if (aiBehavior->behavior == 1) {
                    std::cerr << "Entity " << i << " has behavior 1" << std::endl;   
                }
            }
        } catch (std::exception &e) {
            std::cerr << "IA Error: " << e.what() << std::endl;
        }
    } catch (std::exception &e) {
        std::cerr << " IA Error: " << e.what() << std::endl;
    }
}

void Systems::BehaviorSystem::init(Engine::GameEngine &engine)
{
    if (!engine.registerComponent<Components::Ai>("./plugins/bin/components/", "Ai"))
        std::cerr << "Error: Could not register Ai component in system Input" << std::endl;
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
