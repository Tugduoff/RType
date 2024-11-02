/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Menu.cpp file
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
#include "../SpriteComponent.hpp"
#include "library_entrypoint.hpp"
#include <iostream>
#include <stdexcept>
#include "../Text.hpp"

#define TM 6
#define letter_R 5
#define dot 4
#define letter_T 3
#define letter_Y 2
#define letter_P 1
#define letter_E 0

Systems::Menu::Menu(libconfig::Setting &)
{
}

size_t Systems::Menu::getIndexInVector(std::vector<size_t> &vec, size_t &e) {
    for (size_t i = 0; i < vec.size(); i++) {
        if (vec[i] == e)
            return i;
    }
    return -1;
}

void Systems::Menu::destroyEntity(Engine::GameEngine &engine) {
    for (size_t i =0; i < 12; i++) {
        engine.getRegistry().killEntity(static_cast<ECS::Entity>(i));
    }
}

void Systems::Menu::run(Engine::GameEngine &engine, sf::RenderWindow &window)
{
    if (!engine.isInMenu())
        return;

    auto &reg = engine.getRegistry();
    auto &posArr = reg.componentManager().getComponents<Components::Position>();
    auto &velArr = reg.componentManager().getComponents<Components::Velocity>();
    auto &textArr = reg.componentManager().getComponents<Components::Text>();

    auto &controllableArr = reg.componentManager().getComponents<Components::Controllable>();

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
            }
        }
    }

    try {
        auto &ctrl = controllableArr[__selectManager];

        bool &inputForward = ctrl->inputs[(int)Action::FORWARD];
        bool &inputBackward = ctrl->inputs[(int)Action::BACKWARD];
        bool &inputRight = ctrl->inputs[(int)Action::RIGHT];
        bool &inputLeft = ctrl->inputs[(int)Action::LEFT];
        bool &inputEnter = ctrl->inputs[(int)Action::ACTION10];

        size_t currentIndex = getIndexInVector(__controllableTexts, __selectedText);

        if (inputForward) {
            if (currentIndex != 0)
                __selectedText = __controllableTexts[currentIndex - 1];
            else 
                __selectedText = __controllableTexts[__controllableTexts.size() - 1];
            inputForward = false;
        }

        if (inputBackward) {
            if (currentIndex != __controllableTexts.size() - 1)
                __selectedText = __controllableTexts[currentIndex + 1];
            else
                __selectedText = __controllableTexts[0];
            inputBackward = false;
        }

        if (inputEnter) {
            auto &text = textArr[__controllableTexts[currentIndex]];
            if (text->textContent == "PLAY") {
                std::cout << "START GAME" << std::endl;
                engine._inMenu = false;
                destroyEntity(engine);
                return;
            }
        }

    } catch (std::exception &) {}

    for (auto entity : __controllableTexts) {
        try {
            auto &text = textArr[entity];
            if (__selectedText == entity) {
                text->setColor(sf::Color::Red);
            } else {
                text->setColor(sf::Color::White);
            }

        } catch (std::exception &) {}
    }
}

void Systems::Menu::init(Engine::GameEngine &engine)
{
    auto &manager = engine.getRegistry().componentManager();

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
        {Action::ACTION10, Key::ENTER}
    };

    ECS::Entity Title_E = engine.getRegistry().createEntity();
    manager.addComponent<Components::Position>(Title_E, engine.newComponent<Components::Position>(2000, 540, 2));
    manager.addComponent<Components::Velocity>(Title_E, engine.newComponent<Components::Velocity>(-2, 0, 0));
    manager.addComponent<Components::Scale>(Title_E, engine.newComponent<Components::Scale>(150, 150));
    manager.addComponent<Components::SpriteID>(Title_E, engine.newComponent<Components::SpriteID>(static_cast<std::string>("title_e")));
    _entities.push_back(Title_E);

    ECS::Entity Title_P = engine.getRegistry().createEntity();
    manager.addComponent<Components::Position>(Title_P, engine.newComponent<Components::Position>(2000, 540, 2));
    manager.addComponent<Components::Velocity>(Title_P, engine.newComponent<Components::Velocity>(-2, 0, 0));
    manager.addComponent<Components::Scale>(Title_P, engine.newComponent<Components::Scale>(150, 150));
    manager.addComponent<Components::SpriteID>(Title_P, engine.newComponent<Components::SpriteID>(static_cast<std::string>("title_p")));
    _entities.push_back(Title_P);

    ECS::Entity Title_Y = engine.getRegistry().createEntity();
    manager.addComponent<Components::Position>(Title_Y, engine.newComponent<Components::Position>(2000, 540, 2));
    manager.addComponent<Components::Velocity>(Title_Y, engine.newComponent<Components::Velocity>(-2, 0, 0));
    manager.addComponent<Components::Scale>(Title_Y, engine.newComponent<Components::Scale>(150, 150));
    manager.addComponent<Components::SpriteID>(Title_Y, engine.newComponent<Components::SpriteID>(static_cast<std::string>("title_y")));
    _entities.push_back(Title_Y);

    ECS::Entity Title_T = engine.getRegistry().createEntity();
    manager.addComponent<Components::Position>(Title_T, engine.newComponent<Components::Position>(2000, 540, 2));
    manager.addComponent<Components::Velocity>(Title_T, engine.newComponent<Components::Velocity>(-2, 0, 0));
    manager.addComponent<Components::Scale>(Title_T, engine.newComponent<Components::Scale>(150, 150));
    manager.addComponent<Components::SpriteID>(Title_T, engine.newComponent<Components::SpriteID>(static_cast<std::string>("title_t")));
    _entities.push_back(Title_T);

    ECS::Entity Title_o = engine.getRegistry().createEntity();
    manager.addComponent<Components::Position>(Title_o, engine.newComponent<Components::Position>(2000, 540, 2));
    manager.addComponent<Components::Velocity>(Title_o, engine.newComponent<Components::Velocity>(-2, 0, 0));
    manager.addComponent<Components::Scale>(Title_o, engine.newComponent<Components::Scale>(150, 150));
    manager.addComponent<Components::SpriteID>(Title_o, engine.newComponent<Components::SpriteID>(static_cast<std::string>("title_o")));
    _entities.push_back(Title_o);

    ECS::Entity Title_R = engine.getRegistry().createEntity();
    manager.addComponent<Components::Position>(Title_R, engine.newComponent<Components::Position>(2000, 540, 2));
    manager.addComponent<Components::Velocity>(Title_R, engine.newComponent<Components::Velocity>(-2, 0, 0));
    manager.addComponent<Components::Scale>(Title_R, engine.newComponent<Components::Scale>(150, 150));
    manager.addComponent<Components::SpriteID>(Title_R, engine.newComponent<Components::SpriteID>(static_cast<std::string>("title_r")));
    _entities.push_back(Title_R);

    ECS::Entity Title_TM = engine.getRegistry().createEntity();
    manager.addComponent<Components::Position>(Title_TM, engine.newComponent<Components::Position>(2000, 540, 1));
    manager.addComponent<Components::Velocity>(Title_TM, engine.newComponent<Components::Velocity>(0, 0, 0));
    manager.addComponent<Components::Scale>(Title_TM, engine.newComponent<Components::Scale>(300, 300));
    manager.addComponent<Components::SpriteID>(Title_TM, engine.newComponent<Components::SpriteID>(static_cast<std::string>("title_tm")));
    _entities.push_back(Title_TM);

    ECS::Entity textPlayers = engine.getRegistry().createEntity();
    manager.addComponent<Components::Position>(textPlayers, engine.newComponent<Components::Position>(1500, 300, 2));
    std::unique_ptr<Components::Text> tmpPlayers = std::make_unique<Components::Text>("PLAYERS < 1 >", 50, sf::Color::White);
    manager.addComponent<Components::Text>(textPlayers, std::move(tmpPlayers));
    __controllableTexts.push_back(textPlayers);
    _entities.push_back(textPlayers);

    ECS::Entity textPlay = engine.getRegistry().createEntity();
    manager.addComponent<Components::Position>(textPlay, engine.newComponent<Components::Position>(1500, 450, 2));
    std::unique_ptr<Components::Text> tmpPlay = std::make_unique<Components::Text>("PLAY", 50, sf::Color::Red);
    manager.addComponent<Components::Text>(textPlay, std::move(tmpPlay));
    __controllableTexts.push_back(textPlay);
    _entities.push_back(textPlay);

    ECS::Entity textLevelEditor = engine.getRegistry().createEntity();
    manager.addComponent<Components::Position>(textLevelEditor, engine.newComponent<Components::Position>(1500, 600, 2));
    std::unique_ptr<Components::Text> tmpLevelEditor = std::make_unique<Components::Text>("LEVEL EDITOR", 50, sf::Color::White);
    manager.addComponent<Components::Text>(textLevelEditor, std::move(tmpLevelEditor));
    __controllableTexts.push_back(textLevelEditor);
    _entities.push_back(textLevelEditor);

    ECS::Entity textOptions = engine.getRegistry().createEntity();
    manager.addComponent<Components::Position>(textOptions, engine.newComponent<Components::Position>(1500, 750, 2));
    std::unique_ptr<Components::Text> tmpOptions = std::make_unique<Components::Text>("OPTIONS", 50, sf::Color::White);
    manager.addComponent<Components::Text>(textOptions, std::move(tmpOptions));
    __controllableTexts.push_back(textOptions);
    _entities.push_back(textOptions);


    ECS::Entity controlManager = engine.getRegistry().createEntity();
    manager.addComponent<Components::Controllable>(controlManager, engine.newComponent<Components::Controllable>(keyBindings));
    _entities.push_back(controlManager);

    __selectedText = textPlay;
    __selectManager = controlManager;
}
