/*
** EPITECH PROJECT, 2024
** RType
** File description:
** MoveSystem.cpp file
*/

#include "ECS/registry/Registry.hpp"
#include "GameEngine/GameEngine.hpp"
#include "Move.hpp"
#include "plugins/components/position/Position.hpp"
#include "plugins/components/velocity/Velocity.hpp"
#include <iostream>
#include <stdexcept>
#include <memory>

void Systems::MoveSystem::run(ECS::Registry &reg)
{
    try {
        auto &posComponents = reg.componentManager().getComponents<Components::Position>();
        auto &velComponents = reg.componentManager().getComponents<Components::Velocity>();

        size_t i = 0;
        for (i = 0; i < posComponents.size() || i < velComponents.size(); i++) {
            auto &pos = posComponents[i];
            auto &vel = velComponents[i];

            if (!pos || !vel)
                continue;
            pos->x += vel->x;
            pos->y += vel->y;
        }
    } catch (std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void Systems::MoveSystem::init(const ECS::ComponentRegisterer &registerer)
{
    if (!registerer.registerComponent<Components::Position>("./plugins/bin/components/libPosition.so"))
        std::cerr << "Error: Could not register Position component in system Move" << std::endl;
    if (!registerer.registerComponent<Components::Velocity>("./plugins/bin/components/libVelocity.so"))
        std::cerr << "Error: Could not register Velocity component in system Move" << std::endl;
}

extern "C" std::unique_ptr<Systems::ISystem> entryPoint()
{
    return std::make_unique<Systems::MoveSystem>();
}
