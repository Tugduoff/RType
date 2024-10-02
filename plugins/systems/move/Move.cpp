/*
** EPITECH PROJECT, 2024
** RType
** File description:
** MoveSystem.cpp file
*/

#include "GameEngine/GameEngine.hpp"
#include "Move.hpp"
#include "plugins/components/position/Position.hpp"
#include "plugins/components/velocity/Velocity.hpp"
#include <iostream>
#include <stdexcept>
#include <memory>
#include <cstdint>

void Systems::MoveSystem::run(Engine::GameEngine &engine)
{
    auto &reg = engine.getRegistry();

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

void Systems::MoveSystem::init(Engine::GameEngine &engine)
{
    if (!engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position"))
        std::cerr << "Error: Could not register Position component in system Move" << std::endl;
    if (!engine.registerComponent<Components::Velocity>("./plugins/bin/components/", "Velocity"))
        std::cerr << "Error: Could not register Velocity component in system Move" << std::endl;
}

extern "C"
{
#ifdef _WIN32
    __declspec(dllexport)
#endif
    Systems::ISystem *entryPoint()
    {
        return new Systems::MoveSystem();
    }
}
