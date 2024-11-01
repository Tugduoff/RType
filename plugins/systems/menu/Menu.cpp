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
#include "components/velocity/Velocity.hpp"
#include "components/spriteId/SpriteID.hpp"
#include "components/deathRange/DeathRange.hpp"
#include "systems/sfml/SpriteComponent.hpp"
#include "library_entrypoint.hpp"
#include <iostream>
#include <stdexcept>

#define letter_R 5
#define dot 4
#define letter_T 3
#define letter_Y 2
#define letter_P 1
#define letter_E 0

Systems::MenuSystem::MenuSystem(libconfig::Setting &)
{
}

void Systems::MenuSystem::run(Engine::GameEngine &engine)
{
    if (!engine.isInMenu())
        return;

    auto &reg = engine.getRegistry();
    auto &posArr = reg.componentManager().getComponents<Components::Position>();
    auto &velArr = reg.componentManager().getComponents<Components::Velocity>();

    for (auto &&[i, pos, vel] : IndexedZipper(posArr, velArr)) {
        if (i <= 5 && pos.x < 200) {
            if (!_titleReachedLeftEdge) {
                vel.x = 0;
                if (i == 5)
                    _titleReachedLeftEdge = true;
            }
        }

        if (i <= 5 && _titleReachedLeftEdge && !_titleDeployed) {
            switch (i) {
                case dot:
                    vel.x = (pos.x < 400) ? 1 : 0;
                    break;
                case letter_T:
                    vel.x = (pos.x < 600) ? 2 : 0;
                    break;
                case letter_Y:
                    vel.x = (pos.x < 800) ? 3 : 0;
                    break;
                case letter_P:
                    vel.x = (pos.x < 1000) ? 4 : 0;
                    break;
                case letter_E:
                    vel.x = (pos.x < 1200) ? 5 : 0;
                    if (pos.x >= 1200)
                        _titleDeployed = true;
                    break;
                default:
                    break;
            }
        }

        if (i <= 5 && _titleReachedLeftEdge && _titleDeployed) {
            vel.x = 0;
            vel.y = (pos.y < 900) ? 2 : 0;
        }
    }
}


void Systems::MenuSystem::init(Engine::GameEngine &engine)
{
    engine._inMenu = true;
    _titleReachedLeftEdge = false;
    _titleDeployed = false;
    // attach to entity from componentManager
    engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position");
    engine.registerComponent<Components::Velocity>("./plugins/bin/components/", "Velocity");
    engine.registerComponent<Components::SpriteID>("./plugins/bin/components/", "SpriteID");

    ECS::Entity Title_E = engine.getRegistry().createEntity();
    engine.getRegistry().componentManager().addComponent<Components::Position>(Title_E, engine.newComponent<Components::Position>(2000, 540, 2));
    engine.getRegistry().componentManager().addComponent<Components::Velocity>(Title_E, engine.newComponent<Components::Velocity>(-2, 0, 0));
    engine.getRegistry().componentManager().addComponent<Components::SpriteID>(Title_E, engine.newComponent<Components::SpriteID>(static_cast<std::string>("title_e")));

    ECS::Entity Title_P = engine.getRegistry().createEntity();
    engine.getRegistry().componentManager().addComponent<Components::Position>(Title_P, engine.newComponent<Components::Position>(2000, 540, 2));
    engine.getRegistry().componentManager().addComponent<Components::Velocity>(Title_P, engine.newComponent<Components::Velocity>(-2, 0, 0));
    engine.getRegistry().componentManager().addComponent<Components::SpriteID>(Title_P, engine.newComponent<Components::SpriteID>(static_cast<std::string>("title_p")));

    ECS::Entity Title_Y = engine.getRegistry().createEntity();
    engine.getRegistry().componentManager().addComponent<Components::Position>(Title_Y, engine.newComponent<Components::Position>(2000, 540, 2));
    engine.getRegistry().componentManager().addComponent<Components::Velocity>(Title_Y, engine.newComponent<Components::Velocity>(-2, 0, 0));
    engine.getRegistry().componentManager().addComponent<Components::SpriteID>(Title_Y, engine.newComponent<Components::SpriteID>(static_cast<std::string>("title_y")));

    ECS::Entity Title_T = engine.getRegistry().createEntity();
    engine.getRegistry().componentManager().addComponent<Components::Position>(Title_T, engine.newComponent<Components::Position>(2000, 540, 2));
    engine.getRegistry().componentManager().addComponent<Components::Velocity>(Title_T, engine.newComponent<Components::Velocity>(-2, 0, 0));
    engine.getRegistry().componentManager().addComponent<Components::SpriteID>(Title_T, engine.newComponent<Components::SpriteID>(static_cast<std::string>("title_t")));

    ECS::Entity Title_o = engine.getRegistry().createEntity();
    engine.getRegistry().componentManager().addComponent<Components::Position>(Title_o, engine.newComponent<Components::Position>(2000, 540, 2));
    engine.getRegistry().componentManager().addComponent<Components::Velocity>(Title_o, engine.newComponent<Components::Velocity>(-2, 0, 0));
    engine.getRegistry().componentManager().addComponent<Components::SpriteID>(Title_o, engine.newComponent<Components::SpriteID>(static_cast<std::string>("title_o")));

    ECS::Entity Title_R = engine.getRegistry().createEntity();
    engine.getRegistry().componentManager().addComponent<Components::Position>(Title_R, engine.newComponent<Components::Position>(2000, 540, 2));
    engine.getRegistry().componentManager().addComponent<Components::Velocity>(Title_R, engine.newComponent<Components::Velocity>(-2, 0, 0));
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
