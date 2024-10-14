/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Input.cpp file
*/

#define NOMINMAX
#include "components/position/Position.hpp"
#include "components/spriteId/SpriteID.hpp"
#include "SpriteComponent.hpp"
#include "Input.hpp"

Systems::Input::Input(libconfig::Setting &)
{
}

Systems::Input::Input()
{
}

void Systems::Input::init(Engine::GameEngine &engine)
{
    if (!engine.registerComponent<Components::Controllable>("./plugins/bin/components/", "Controllable"))
        std::cerr << "Error: Could not register Controllable component in system Input" << std::endl;
}

void Systems::Input::run(Engine::GameEngine &engine, sf::RenderWindow &window)
{
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            std::cerr << "Window closed" << std::endl;
            window.close();
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                std::cerr << "Escape pressed" << std::endl;
                window.close();
            }
        }
        handleInput(engine, event);
        std::cout << std::endl;
    }
}

void Systems::Input::handleInput(Engine::GameEngine &engine, sf::Event &event)
{
    auto &ctrlComponents = engine.getRegistry().componentManager().getComponents<Components::Controllable>();

    if (event.type == sf::Event::KeyPressed) {
        for (auto &ctrl : ctrlComponents) {
            Key key = Key::UNKNOWN;
            try {
                key = __sfmlToKey.at(event.key.code);
            } catch (std::exception &e) {
                std::cerr << "Key not handled" << std::endl;
            }
            int index = 0;
            for (auto &[action, actionKey] : ctrl->keyBindings) {
                if (actionKey == key) {
                    handleInputPressed(ctrl->inputs, ctrl->actions, index);
                    return;
                }
                index++;
            }
        }
    }
    if (event.type == sf::Event::KeyReleased) {
        for (auto &ctrl : ctrlComponents) {
            Key key = Key::UNKNOWN;
            try {
                key = __sfmlToKey.at(event.key.code);
            } catch (std::exception &e) {
                std::cerr << "Key not handled" << std::endl;
            }
            int index = 0;
            for (auto &[action, actionKey] : ctrl->keyBindings) {
                if (actionKey == key) {
                    handleInputReleased(ctrl->inputs, ctrl->actions, index);
                    return;
                }
                index++;
            }
        }
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        for (auto &ctrl : ctrlComponents) {
            int index = 0;
            for (auto &[action, actionKey] : ctrl->keyBindings) {
                if ((actionKey == Key::LEFT_CLICK && event.mouseButton.button == sf::Mouse::Left) ||
                    (actionKey == Key::RIGHT_CLICK && event.mouseButton.button == sf::Mouse::Right) ||
                    (actionKey == Key::MIDDLE_CLICK && event.mouseButton.button == sf::Mouse::Middle)) {
                        handleInputPressed(ctrl->inputs, ctrl->actions, index);
                        return;
                }
                index++;
            }
        }
    }
    if (event.type == sf::Event::MouseButtonReleased) {
        for (auto &ctrl : ctrlComponents) {
            int index = 0;
            for (auto &[action, actionKey] : ctrl->keyBindings) {
                if ((actionKey == Key::LEFT_CLICK && event.mouseButton.button == sf::Mouse::Left) ||
                    (actionKey == Key::RIGHT_CLICK && event.mouseButton.button == sf::Mouse::Right) ||
                    (actionKey == Key::MIDDLE_CLICK && event.mouseButton.button == sf::Mouse::Middle)) {
                        handleInputReleased(ctrl->inputs, ctrl->actions, index);
                        return;
                }
                index++;
            }
        }
    }
}

void Systems::Input::handleInputPressed(std::array<bool, 4> &inputs, std::array<bool, 10> &actions, int index)
{
    if (index < 4) {
        inputs[index] = true;
    } else {
        actions[index - 4] = true;
    }
}

void Systems::Input::handleInputReleased(std::array<bool, 4> &inputs, std::array<bool, 10> &actions, int index)
{
    if (index < 4) {
        inputs[index] = false;
    } else {
        actions[index - 4] = false;
    }
}
