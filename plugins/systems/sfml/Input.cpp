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
        try {
            handleInput(engine, event);
        } catch (std::exception &e) {
            std::cerr << "Input handleInput Error: " << e.what() << std::endl;
        }
        std::cout << std::endl;
    }
}

void Systems::Input::handleInput(Engine::GameEngine &engine, sf::Event &event)
{
    auto &ctrlComponents = engine.getRegistry().componentManager().getComponents<Components::Controllable>();   
    auto &spriteComponents = engine.getRegistry().componentManager().getComponents<Components::SpriteComponent>();

    if (event.type == sf::Event::KeyPressed) {
        for (size_t i = 0; i < ctrlComponents.size(); i++) {
            try {
                auto &ctrl = ctrlComponents[i];
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
                        try {
                            auto &sprite = spriteComponents[i];

                            bool forwardInput = ctrl->inputs[(int)Action::FORWARD];
                            bool backwardInput = ctrl->inputs[(int)Action::BACKWARD];
                            bool rightInput = ctrl->inputs[(int)Action::RIGHT];
                            bool leftInput = ctrl->inputs[(int)Action::LEFT];

                            if ((action == Action::FORWARD && rightInput) ||
                                (action == Action::RIGHT && forwardInput))
                                sprite->loadTextureForAction("FORWARD_RIGHT");
                            else if ((action == Action::FORWARD && leftInput) ||
                                (action == Action::LEFT && forwardInput))
                                sprite->loadTextureForAction("FORWARD_LEFT");
                            else if ((action == Action::BACKWARD && rightInput) ||
                                (action == Action::RIGHT && backwardInput))
                                sprite->loadTextureForAction("BACKWARD_RIGHT");
                            else if ((action == Action::BACKWARD && leftInput) ||
                                (action == Action::LEFT && backwardInput))
                                sprite->loadTextureForAction("BACKWARD_LEFT");
                            else if (action == Action::FORWARD)
                                sprite->loadTextureForAction("FORWARD");
                            else if (action == Action::BACKWARD)
                                sprite->loadTextureForAction("BACKWARD");
                            else if (action == Action::RIGHT)
                                sprite->loadTextureForAction("RIGHT");
                            else if (action == Action::LEFT)
                                sprite->loadTextureForAction("LEFT");
                        } catch (std::exception &e) {
                            std::cerr << "Sprite not found" << std::endl;
                        }
                        return;
                    }
                    index++;
                }
            } catch (std::exception &e) {
                continue;
            }
        }
    }
    if (event.type == sf::Event::KeyReleased) {
        for (size_t i = 0; i < ctrlComponents.size(); i++) {
            try {
                auto &ctrl = ctrlComponents[i];
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
                        try {
                            auto &sprite = spriteComponents[i];

                            bool forwardInput = ctrl->inputs[(int)Action::FORWARD];
                            bool backwardInput = ctrl->inputs[(int)Action::BACKWARD];
                            bool rightInput = ctrl->inputs[(int)Action::RIGHT];
                            bool leftInput = ctrl->inputs[(int)Action::LEFT];

                            if ((action == Action::FORWARD && rightInput) ||
                                (action == Action::BACKWARD && rightInput))
                                sprite->loadTextureForAction("RIGHT");
                            else if ((action == Action::FORWARD && leftInput) ||
                                (action == Action::BACKWARD && leftInput))
                                sprite->loadTextureForAction("LEFT");
                            else if ((action == Action::RIGHT && forwardInput) ||
                                (action == Action::LEFT && forwardInput))
                                sprite->loadTextureForAction("FORWARD");
                            else if ((action == Action::RIGHT && backwardInput) ||
                                (action == Action::LEFT && backwardInput))
                                sprite->loadTextureForAction("BACKWARD");
                            else
                                sprite->loadTextureForAction("IDLE");
                        } catch (std::exception &e) {
                            std::cerr << "Sprite Component not found" << std::endl;
                        }
                        return;
                    }
                    index++;
                }
            } catch (std::exception &e) {
                continue;
            }
        }
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        for (size_t i = 0; i < ctrlComponents.size(); i++) {
            try {
                auto &ctrl = ctrlComponents[i];
                int index = 0;
                for (auto &[action, actionKey] : ctrl->keyBindings) {
                    if ((actionKey == Key::LEFT_CLICK && event.mouseButton.button == sf::Mouse::Left) ||
                        (actionKey == Key::RIGHT_CLICK && event.mouseButton.button == sf::Mouse::Right) ||
                        (actionKey == Key::MIDDLE_CLICK && event.mouseButton.button == sf::Mouse::Middle)) {
                            handleInputPressed(ctrl->inputs, ctrl->actions, index);
                            try {
                                auto &sprite = spriteComponents[i];
                                
                                bool forwardInput = ctrl->inputs[(int)Action::FORWARD];
                                bool backwardInput = ctrl->inputs[(int)Action::BACKWARD];
                                bool rightInput = ctrl->inputs[(int)Action::RIGHT];
                                bool leftInput = ctrl->inputs[(int)Action::LEFT];

                                if ((action == Action::FORWARD && rightInput) ||
                                    (action == Action::RIGHT && forwardInput))
                                    sprite->loadTextureForAction("FORWARD_RIGHT");
                                else if ((action == Action::FORWARD && leftInput) ||
                                    (action == Action::LEFT && forwardInput))
                                    sprite->loadTextureForAction("FORWARD_LEFT");
                                else if ((action == Action::BACKWARD && rightInput) ||
                                    (action == Action::RIGHT && backwardInput))
                                    sprite->loadTextureForAction("BACKWARD_RIGHT");
                                else if ((action == Action::BACKWARD && leftInput) ||
                                    (action == Action::LEFT && backwardInput))
                                    sprite->loadTextureForAction("BACKWARD_LEFT");
                                else if (action == Action::FORWARD)
                                    sprite->loadTextureForAction("FORWARD");
                                else if (action == Action::BACKWARD)
                                    sprite->loadTextureForAction("BACKWARD");
                                else if (action == Action::RIGHT)
                                    sprite->loadTextureForAction("RIGHT");
                                else if (action == Action::LEFT)
                                    sprite->loadTextureForAction("LEFT");
                            } catch (std::exception &e) {
                                std::cerr << "Sprite Component not found" << std::endl;
                            }
                            return;
                    }
                    index++;
                }
            } catch (std::exception &e) {
                continue;
            }
        }
    }
    if (event.type == sf::Event::MouseButtonReleased) {
        for (size_t i = 0; i < ctrlComponents.size(); i++) {
            try {
                auto &ctrl = ctrlComponents[i];
                int index = 0;
                for (auto &[action, actionKey] : ctrl->keyBindings) {
                    if ((actionKey == Key::LEFT_CLICK && event.mouseButton.button == sf::Mouse::Left) ||
                        (actionKey == Key::RIGHT_CLICK && event.mouseButton.button == sf::Mouse::Right) ||
                        (actionKey == Key::MIDDLE_CLICK && event.mouseButton.button == sf::Mouse::Middle)) {
                            handleInputReleased(ctrl->inputs, ctrl->actions, index);
                            try {
                                auto &sprite = spriteComponents[i];

                                bool forwardInput = ctrl->inputs[(int)Action::FORWARD];
                                bool backwardInput = ctrl->inputs[(int)Action::BACKWARD];
                                bool rightInput = ctrl->inputs[(int)Action::RIGHT];
                                bool leftInput = ctrl->inputs[(int)Action::LEFT];

                                if ((action == Action::FORWARD && rightInput) ||
                                    (action == Action::BACKWARD && rightInput))
                                    sprite->loadTextureForAction("RIGHT");
                                else if ((action == Action::FORWARD && leftInput) ||
                                    (action == Action::BACKWARD && leftInput))
                                    sprite->loadTextureForAction("LEFT");
                                else if ((action == Action::RIGHT && forwardInput) ||
                                    (action == Action::LEFT && forwardInput))
                                    sprite->loadTextureForAction("FORWARD");
                                else if ((action == Action::RIGHT && backwardInput) ||
                                    (action == Action::LEFT && backwardInput))
                                    sprite->loadTextureForAction("BACKWARD");
                                else
                                    sprite->loadTextureForAction("IDLE");
                            } catch (std::exception &e) {
                                std::cerr << "Sprite Component not found" << std::endl;
                            }
                            return;
                    }
                    index++;
                }
            } catch (std::exception &e) {
                continue;
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
