/*
** EPITECH PROJECT, 2024
** RType
** File description:
** MenuSystem.cpp file
*/

#include "GameEngine/GameEngine.hpp"
#include "ECS/utilities/Zipper/IndexedZipper.hpp"
#include "Menu.hpp"
#include "components/position/Position.hpp"
#include "components/spriteId/SpriteID.hpp"
#include "components/deathRange/DeathRange.hpp"
#include "systems/sfml/SpriteComponent.hpp"
#include "library_entrypoint.hpp"
#include <iostream>
#include <stdexcept>

Systems::MenuSystem::MenuSystem(libconfig::Setting &)
{
}

void Systems::MenuSystem::run(Engine::GameEngine &engine)
{
    if (!engine.isInMenu())
        return;

    auto &reg = engine.getRegistry();
    auto &posArr = reg.componentManager().getComponents<Components::Position>();

    std::cout<<"Menu running"<<std::endl;

    for (auto &&[i, pos] : IndexedZipper(posArr)) {
        std::cout << "[" << i << "] " << pos.x << " | " << pos.y << std::endl;
    }
}

void Systems::MenuSystem::init(Engine::GameEngine &engine)
{
    engine._inMenu = true;
    // attach to entity from componentManager
    engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position");
    engine.registerComponent<Components::SpriteID>("./plugins/bin/components/", "SpriteID");

    ECS::Entity Title_R = engine.getRegistry().createEntity();
    engine.getRegistry().componentManager().addComponent<Components::Position>(Title_R, engine.newComponent<Components::Position>(800, 600, 2));
    engine.getRegistry().componentManager().addComponent<Components::SpriteID>(Title_R, engine.newComponent<Components::SpriteID>(static_cast<std::string>("title_r")));
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryPoint()
{
    return new Systems::MenuSystem();
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryConfig(libconfig::Setting &config)
{
    return new Systems::MenuSystem(config);
}
