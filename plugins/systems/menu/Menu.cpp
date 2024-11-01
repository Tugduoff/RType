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
#include "components/scale/Scale.hpp"
#include "components/controllable/Controllable.hpp"
#include "components/spriteId/SpriteID.hpp"
#include "components/deathRange/DeathRange.hpp"
#include "systems/sfml/SpriteComponent.hpp"
#include "library_entrypoint.hpp"
#include <iostream>
#include <stdexcept>

#define TM 6
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
    // auto &scaleArr = reg.componentManager().getComponents<Components::Scale>();

    auto &controllableArr = reg.componentManager().getComponents<Components::Controllable>();

    // for (auto &&[i, pos, vel, scale, controllable] : IndexedZipper(posArr, velArr, scaleArr, controllableArr)) {
    for (auto &&[i, pos, vel, controllable] : IndexedZipper(posArr, velArr, controllableArr)) {
    // for (auto &&[i, pos, vel] : IndexedZipper(posArr, velArr)) {
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

        if (i == TM && _titleReachedLeftEdge && _titleDeployed && !_tmSpawned) {
            pos.x = 200;
            pos.y = 400;
            _tmSpawned = true;
        }

        if (_titleReachedLeftEdge && _titleDeployed) {
            vel.x = 0;
            if (i <= 5) {
                vel.y = (pos.y < 700) ? 2 : 0;
            }

            if (i == TM) {
                vel.x = (pos.x < 1300) ? 6 : 0;
                vel.y = (pos.y < 800) ? 2 : 0;

                // bool inputForward = controllable.inputs[(int)Action::FORWARD];
                // bool inputBackward = controllable.inputs[(int)Action::BACKWARD];
                // bool inputRight = controllable.inputs[(int)Action::RIGHT];
                // bool inputLeft = controllable.inputs[(int)Action::LEFT];
                // bool inputLeftClick = controllable.actions[(int)Action::ACTION1 - 4];
                // bool inputRightClick = controllable.actions[(int)Action::ACTION2 - 4];

                // if (inputLeftClick) {
                //     std::cout << "Left Click triggered" << std::endl;
                //     exit(0);
                // }
                // if (inputRightClick) {
                //     std::cout << "Right Click triggered" << std::endl;
                //     exit(0);
                // }
                // if (inputForward && !inputBackward) {
                //     std::cout << "Forward triggered" << std::endl;
                //     exit(0);
                // }
                // if (inputBackward && !inputForward) {
                //     std::cout << "Backward triggered" << std::endl;
                //     exit(0);
                // }
                // if (inputRight && !inputLeft) {
                //     std::cout << "Right triggered" << std::endl;
                //     exit(0);
                // }
                // if (inputLeft && !inputRight) {
                //     std::cout << "Left triggered" << std::endl;
                //     exit(0);
                // }
            }
        }
    }
}

void Systems::MenuSystem::init(Engine::GameEngine &engine)
{
    engine._inMenu = true;
    _titleReachedLeftEdge = false;
    _titleDeployed = false;
    _tmSpawned = false;
    // attach to entity from componentManager
    engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position");
    engine.registerComponent<Components::Velocity>("./plugins/bin/components/", "Velocity");
    engine.registerComponent<Components::Scale>("./plugins/bin/components/", "Scale");
    engine.registerComponent<Components::SpriteID>("./plugins/bin/components/", "SpriteID");
    engine.registerComponent<Components::Controllable>("./plugins/bin/components/", "Controllable");

    std::map<enum Action, enum Key> keyBindings = {
        {Action::FORWARD, Key::Z},
        {Action::BACKWARD, Key::S},
        {Action::LEFT, Key::Q},
        {Action::RIGHT, Key::D},
        {Action::ACTION1, Key::LEFT_CLICK},
        {Action::ACTION2, Key::RIGHT_CLICK},
        {Action::ACTION3, Key::MIDDLE_CLICK},
        {Action::ACTION4, Key::NUM_0},
        {Action::ACTION5, Key::NUM_1},
        {Action::ACTION6, Key::NUM_2},
        {Action::ACTION7, Key::NUM_3},
        {Action::ACTION8, Key::NUM_4},
        {Action::ACTION9, Key::NUM_5},
        {Action::ACTION10, Key::NUM_6}
    };

    ECS::Entity Title_E = engine.getRegistry().createEntity();
    engine.getRegistry().componentManager().addComponent<Components::Position>(Title_E, engine.newComponent<Components::Position>(2000, 540, 2));
    engine.getRegistry().componentManager().addComponent<Components::Velocity>(Title_E, engine.newComponent<Components::Velocity>(-2, 0, 0));
    engine.getRegistry().componentManager().addComponent<Components::Scale>(Title_E, engine.newComponent<Components::Scale>(150, 150));
    engine.getRegistry().componentManager().addComponent<Components::SpriteID>(Title_E, engine.newComponent<Components::SpriteID>(static_cast<std::string>("title_e")));
    engine.getRegistry().componentManager().addComponent<Components::Controllable>(Title_E, engine.newComponent<Components::Controllable>(keyBindings)); // tmp

    ECS::Entity Title_P = engine.getRegistry().createEntity();
    engine.getRegistry().componentManager().addComponent<Components::Position>(Title_P, engine.newComponent<Components::Position>(2000, 540, 2));
    engine.getRegistry().componentManager().addComponent<Components::Velocity>(Title_P, engine.newComponent<Components::Velocity>(-2, 0, 0));
    engine.getRegistry().componentManager().addComponent<Components::Scale>(Title_P, engine.newComponent<Components::Scale>(150, 150));
    engine.getRegistry().componentManager().addComponent<Components::SpriteID>(Title_P, engine.newComponent<Components::SpriteID>(static_cast<std::string>("title_p")));
    engine.getRegistry().componentManager().addComponent<Components::Controllable>(Title_P, engine.newComponent<Components::Controllable>(keyBindings)); // tmp

    ECS::Entity Title_Y = engine.getRegistry().createEntity();
    engine.getRegistry().componentManager().addComponent<Components::Position>(Title_Y, engine.newComponent<Components::Position>(2000, 540, 2));
    engine.getRegistry().componentManager().addComponent<Components::Velocity>(Title_Y, engine.newComponent<Components::Velocity>(-2, 0, 0));
    engine.getRegistry().componentManager().addComponent<Components::Scale>(Title_Y, engine.newComponent<Components::Scale>(150, 150));
    engine.getRegistry().componentManager().addComponent<Components::SpriteID>(Title_Y, engine.newComponent<Components::SpriteID>(static_cast<std::string>("title_y")));
    engine.getRegistry().componentManager().addComponent<Components::Controllable>(Title_Y, engine.newComponent<Components::Controllable>(keyBindings)); // tmp

    ECS::Entity Title_T = engine.getRegistry().createEntity();
    engine.getRegistry().componentManager().addComponent<Components::Position>(Title_T, engine.newComponent<Components::Position>(2000, 540, 2));
    engine.getRegistry().componentManager().addComponent<Components::Velocity>(Title_T, engine.newComponent<Components::Velocity>(-2, 0, 0));
    engine.getRegistry().componentManager().addComponent<Components::Scale>(Title_T, engine.newComponent<Components::Scale>(150, 150));
    engine.getRegistry().componentManager().addComponent<Components::SpriteID>(Title_T, engine.newComponent<Components::SpriteID>(static_cast<std::string>("title_t")));
    engine.getRegistry().componentManager().addComponent<Components::Controllable>(Title_T, engine.newComponent<Components::Controllable>(keyBindings)); // tmp

    ECS::Entity Title_o = engine.getRegistry().createEntity();
    engine.getRegistry().componentManager().addComponent<Components::Position>(Title_o, engine.newComponent<Components::Position>(2000, 540, 2));
    engine.getRegistry().componentManager().addComponent<Components::Velocity>(Title_o, engine.newComponent<Components::Velocity>(-2, 0, 0));
    engine.getRegistry().componentManager().addComponent<Components::Scale>(Title_o, engine.newComponent<Components::Scale>(150, 150));
    engine.getRegistry().componentManager().addComponent<Components::SpriteID>(Title_o, engine.newComponent<Components::SpriteID>(static_cast<std::string>("title_o")));
    engine.getRegistry().componentManager().addComponent<Components::Controllable>(Title_o, engine.newComponent<Components::Controllable>(keyBindings)); // tmp

    ECS::Entity Title_R = engine.getRegistry().createEntity();
    engine.getRegistry().componentManager().addComponent<Components::Position>(Title_R, engine.newComponent<Components::Position>(2000, 540, 2));
    engine.getRegistry().componentManager().addComponent<Components::Velocity>(Title_R, engine.newComponent<Components::Velocity>(-2, 0, 0));
    engine.getRegistry().componentManager().addComponent<Components::Scale>(Title_R, engine.newComponent<Components::Scale>(150, 150));
    engine.getRegistry().componentManager().addComponent<Components::SpriteID>(Title_R, engine.newComponent<Components::SpriteID>(static_cast<std::string>("title_r")));
    engine.getRegistry().componentManager().addComponent<Components::Controllable>(Title_R, engine.newComponent<Components::Controllable>(keyBindings)); // tmp

    ECS::Entity Title_TM = engine.getRegistry().createEntity();
    engine.getRegistry().componentManager().addComponent<Components::Position>(Title_TM, engine.newComponent<Components::Position>(2000, 540, 2));
    engine.getRegistry().componentManager().addComponent<Components::Velocity>(Title_TM, engine.newComponent<Components::Velocity>(0, 0, 0));
    engine.getRegistry().componentManager().addComponent<Components::Scale>(Title_TM, engine.newComponent<Components::Scale>(300, 300));
    engine.getRegistry().componentManager().addComponent<Components::SpriteID>(Title_TM, engine.newComponent<Components::SpriteID>(static_cast<std::string>("title_tm")));
    engine.getRegistry().componentManager().addComponent<Components::Controllable>(Title_TM, engine.newComponent<Components::Controllable>(keyBindings)); // tmp
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
