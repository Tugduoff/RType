/*
** EPITECH PROJECT, 2024
** RType
** File description:
** MoveSystem.cpp file
*/

#include "GameEngine/GameEngine.hpp"
#include "ECS/utilities/Zipper/IndexedZipper.hpp"
#include "Move.hpp"
#include "components/position/Position.hpp"
#include "components/velocity/Velocity.hpp"
#include "components/deathRange/DeathRange.hpp"
#include "library_entrypoint.hpp"
#include <iostream>
#include <stdexcept>

Systems::MoveSystem::MoveSystem(libconfig::Setting &)
{
}

void Systems::MoveSystem::run(Engine::GameEngine &engine)
{
    auto &reg = engine.getRegistry();

    try {
        auto &posArr = reg.componentManager().getComponents<Components::Position>();
        auto &velArr = reg.componentManager().getComponents<Components::Velocity>();
        auto &dRangeArr = reg.componentManager().getComponents<Components::DeathRange>();


        for (auto &&[i, pos, vel] : IndexedZipper(posArr, velArr)) {
            pos.x += vel.x;
            pos.y += vel.y;
            engine.updateComponent(i, pos.getId(), pos.serialize());
            if (vel.diminishingFactor == 0)
                continue;
            float factor = (float)vel.diminishingFactor / 100;
            vel.floatX *= factor;
            vel.floatY *= factor;
            vel.x = (int)vel.floatX;
            vel.y = (int)vel.floatY;
            engine.updateComponent(i, vel.getId(), vel.serialize());
        }
        for (auto &&[i, pos, drange] : IndexedZipper(posArr, dRangeArr)) {
            if (pos.x > drange.maxX || pos.x < drange.minX ||
                pos.y > drange.maxY || pos.y < drange.minY) {
                reg.killEntity((ECS::Entity)i);
            }
        }
    } catch (std::runtime_error &e) {
        std::cerr << "Move Error: " << e.what() << std::endl;
    }
}

void Systems::MoveSystem::init(Engine::GameEngine &engine)
{
    engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position");
    engine.registerComponent<Components::Velocity>("./plugins/bin/components/", "Velocity");
    engine.registerComponent<Components::DeathRange>("./plugins/bin/components/", "DeathRange");
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryPoint()
{
    return new Systems::MoveSystem();
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryConfig(libconfig::Setting &config)
{
    return new Systems::MoveSystem(config);
}
