/*
** EPITECH PROJECT, 2024
** RType
** File description:
** MoveRaylib.cpp file
*/

#include "GameEngine/GameEngine.hpp"
#include "ECS/utilities/Zipper/IndexedZipper.hpp"
#include "MoveRaylib.hpp"
#include "components/position3d/Position3d.hpp"
#include "components/velocity3d/Velocity3d.hpp"
#include "components/deathRange/DeathRange.hpp"
#include "library_entrypoint.hpp"
#include <iostream>
#include <stdexcept>

Systems::MoveRaylib::MoveRaylib(libconfig::Setting &)
{
}

void Systems::MoveRaylib::run(Engine::GameEngine &engine)
{
    auto &reg = engine.getRegistry();

    try {
        auto &posArr = reg.componentManager().getComponents<Components::Position3d>();
        auto &velArr = reg.componentManager().getComponents<Components::Velocity3d>();
        auto &dRangeArr = reg.componentManager().getComponents<Components::DeathRange>();

        for (auto &&[i, pos, vel] : IndexedZipper(posArr, velArr)) {
            pos.floatX += vel.floatX;
            pos.floatY += vel.floatY;
            pos.floatZ += vel.floatZ;
            if (vel.diminishingFactor == 0)
                continue;
            float factor = (float)vel.diminishingFactor / 100;
            vel.floatX *= factor;
            vel.floatY *= factor;
            vel.floatZ *= factor;
            vel.x = (int)vel.floatX;
            vel.y = (int)vel.floatY;
            vel.z = (int)vel.floatZ;
            pos.x = (int)pos.floatX;
            pos.y = (int)pos.floatY;
            pos.z = (int)pos.floatZ;
            engine.updateComponent(i, pos.getId(), pos.serialize());
            std::cerr << "######### POSITION: " << std::setw(5) << pos.x << " " << std::setw(5) << pos.y << " " << std::setw(5) << pos.z << std::endl;
        }
        for (auto &&[i, pos, drange] : IndexedZipper(posArr, dRangeArr)) {
            if (pos.x > drange.maxX || pos.x < drange.minX ||
                pos.y > drange.maxY || pos.y < drange.minY) {
                reg.killEntity((ECS::Entity)i);
                std::cerr << "Entity " << i << " has been killed due to death range" << std::endl;
            }
        }
    } catch (std::runtime_error &e) {
        std::cerr << "Move Error: " << e.what() << std::endl;
    }
}

void Systems::MoveRaylib::init(Engine::GameEngine &engine)
{
    engine.registerComponent<Components::Position3d>("./plugins/bin/components/", "Position3d");
    engine.registerComponent<Components::Velocity3d>("./plugins/bin/components/", "Velocity3d");
    engine.registerComponent<Components::DeathRange>("./plugins/bin/components/", "DeathRange");
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryPoint()
{
    return new Systems::MoveRaylib();
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryConfig(libconfig::Setting &config)
{
    return new Systems::MoveRaylib(config);
}
