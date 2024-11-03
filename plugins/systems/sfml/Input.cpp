/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Input.cpp file
*/

#define NOMINMAX
#include "components/position/Position.hpp"
#include "components/spriteId/SpriteID.hpp"
#include "components/action/Action.hpp"
#include "Input.hpp"
#include "ECS/utilities/Zipper/IndexedZipper.hpp"

Systems::Input::Input(libconfig::Setting &)
{
}

Systems::Input::Input()
{
}

void Systems::Input::init(Engine::GameEngine &engine)
{
    if (!engine.registerComponent<Components::Controllable>("./plugins/bin/components/", "Controllable"))
        std::cerr << "Error: Could not register Controllable component in system SFML Input" << std::endl;
    if (!engine.registerComponent<Components::ActionComponent>("./plugins/bin/components/", "Action"))
        std::cerr << "Error: Could not register Action component in system SFML Input" << std::endl;
}

void Systems::Input::updateControllable(Engine::GameEngine &engine)
{
    auto &manager = engine.getRegistry().componentManager();

    auto &controllableComponents = manager.getComponents<Components::Controllable>();
    for (auto [i, ctrl] : IndexedZipper(controllableComponents)) {
        std::map<enum Action, enum Key> keyBindings = {
            {Action::FORWARD, Key::Q},
            {Action::BACKWARD, Key::D},
            {Action::LEFT, Key::S},
            {Action::RIGHT, Key::Z},
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
        if (ctrl.keyBindings == keyBindings)
            continue;
        std::cerr << "Updating controllable component with id: " << i << std::endl;
        ctrl.keyBindings = keyBindings;
        engine.updateComponent((ECS::Entity)i, ctrl.getId(), ctrl.serialize());
    }
}

void Systems::Input::run(Engine::GameEngine &engine, sf::RenderWindow &window)
{
    sf::Event event;

    updateControllable(engine);

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

EntityAction Systems::Input::determinePressedAction(
    bool forwardInput,
    bool backwardInput,
    bool rightInput,
    bool leftInput,
    bool shootInput,
    Action action)
{
    if (action == Action::ACTION1 || shootInput) {
        return determinePressedShootAction(forwardInput, backwardInput, rightInput, leftInput);
    }

    // Check for combined movements
    if ((action == Action::FORWARD && rightInput) || (action == Action::RIGHT && forwardInput)) {
        return stringToEntityAction("FORWARD_RIGHT");
    } else if ((action == Action::FORWARD && leftInput) || (action == Action::LEFT && forwardInput)) {
        return stringToEntityAction("FORWARD_LEFT");
    } else if ((action == Action::BACKWARD && rightInput) || (action == Action::RIGHT && backwardInput)) {
        return stringToEntityAction("BACKWARD_RIGHT");
    } else if ((action == Action::BACKWARD && leftInput) || (action == Action::LEFT && backwardInput)) {
        return stringToEntityAction("BACKWARD_LEFT");
    }

    // Check for single direction actions
    switch (action) {
        case Action::FORWARD:
            return stringToEntityAction("FORWARD");
        case Action::BACKWARD:
            return stringToEntityAction("BACKWARD");
        case Action::RIGHT:
            return stringToEntityAction("RIGHT");
        case Action::LEFT:
            return stringToEntityAction("LEFT");
        case Action::ACTION1:
            return determinePressedShootAction(forwardInput, backwardInput, rightInput, leftInput);
        default:
            return EntityAction::IDLE;
    }
}

EntityAction Systems::Input::determineReleasedAction(
    bool forwardInput,
    bool backwardInput,
    bool rightInput,
    bool leftInput,
    bool shootInput,
    Action action)
{
    if (action != Action::ACTION1 && shootInput) {
        return determineReleasedShootAction(forwardInput, backwardInput, rightInput, leftInput, action);
    }
    if (action == Action::ACTION1) {
        if ((forwardInput && rightInput) || (rightInput && forwardInput)) {
            return stringToEntityAction("FORWARD_RIGHT");
        } else if ((forwardInput && leftInput) || (leftInput && forwardInput)) {
            return stringToEntityAction("FORWARD_LEFT");
        } else if ((backwardInput && rightInput) || (rightInput && backwardInput)) {
            return stringToEntityAction("BACKWARD_RIGHT");
        } else if ((backwardInput && leftInput) || (leftInput && backwardInput)) {
            return stringToEntityAction("BACKWARD_LEFT");
        }
        if (forwardInput)
            return EntityAction::FORWARD;
        if (backwardInput)
            return EntityAction::BACKWARD;
        if (rightInput)
            return EntityAction::RIGHT;
        if (leftInput)
            return EntityAction::LEFT;
    }

    // Check for combined movements
    if ((action == Action::FORWARD && rightInput) || (action == Action::BACKWARD && rightInput)) {
        return stringToEntityAction("RIGHT");
    } else if ((action == Action::FORWARD && leftInput) || (action == Action::BACKWARD && leftInput)) {
        return stringToEntityAction("LEFT");
    } else if ((action == Action::RIGHT && forwardInput) || (action == Action::LEFT && forwardInput)) {
        return stringToEntityAction("FORWARD");
    } else if ((action == Action::RIGHT && backwardInput) || (action == Action::LEFT && backwardInput)) {
        return stringToEntityAction("BACKWARD");
    }

    return EntityAction::IDLE;
}

// Function to determine shooting action based on inputs
EntityAction Systems::Input::determinePressedShootAction(bool forwardInput, bool backwardInput, bool rightInput, bool leftInput)
{
    if (rightInput && !leftInput) {
        if (forwardInput && !backwardInput) {
            return stringToEntityAction("SHOOT_FORWARD_RIGHT");
        } else if (!forwardInput && backwardInput) {
            return stringToEntityAction("SHOOT_BACKWARD_RIGHT");
        } else {
            return stringToEntityAction("SHOOT_RIGHT");
        }
    } else if (!rightInput && leftInput) {
        if (forwardInput && !backwardInput) {
            return stringToEntityAction("SHOOT_FORWARD_LEFT");
        } else if (!forwardInput && backwardInput) {
            return stringToEntityAction("SHOOT_BACKWARD_LEFT");
        } else {
            return stringToEntityAction("SHOOT_LEFT");
        }
    } else {
        if (forwardInput && !backwardInput) {
            return stringToEntityAction("SHOOT_FORWARD");
        } else if (!forwardInput && backwardInput) {
            return stringToEntityAction("SHOOT_BACKWARD");
        } else {
            return stringToEntityAction("SHOOT_FORWARD"); // Default shoot forward
        }
    }
}

EntityAction Systems::Input::determineReleasedShootAction(bool forwardInput, bool backwardInput, bool rightInput, bool leftInput, Action action)
{
    if ((action == Action::FORWARD && rightInput) || (action == Action::BACKWARD && rightInput)) {
        return stringToEntityAction("SHOOT_RIGHT");
    } else if ((action == Action::FORWARD && leftInput) || (action == Action::BACKWARD && leftInput)) {
        return stringToEntityAction("SHOOT_LEFT");
    } else if ((action == Action::RIGHT && forwardInput) || (action == Action::LEFT && forwardInput)) {
        return stringToEntityAction("SHOOT_FORWARD");
    } else if ((action == Action::RIGHT && backwardInput) || (action == Action::LEFT && backwardInput)) {
        return stringToEntityAction("SHOOT_BACKWARD");
    }
    return EntityAction::SHOOT_FORWARD;
}

void Systems::Input::handleInput(Engine::GameEngine &engine, sf::Event &event)
{
    auto &ctrlComponents = engine.getRegistry().componentManager().getComponents<Components::Controllable>();   
    auto &entityActionComponents = engine.getRegistry().componentManager().getComponents<Components::ActionComponent>();

    if (event.type == sf::Event::KeyPressed) {
        for (auto &&[i, ctrl] : IndexedZipper(ctrlComponents)) {
            Key key = Key::UNKNOWN;
            try {
                key = __sfmlToKey.at(event.key.code);
            } catch (std::exception &e) {
                std::cerr << "Key not handled" << std::endl;
            }
            int index = 0;
            for (auto &[action, actionKey] : ctrl.keyBindings) {
                if (actionKey == key) {
                    handleInputPressed(ctrl.inputs, ctrl.actions, index);
                    engine.updateComponent(i, ctrl.getId(), ctrl.serialize());
                    try {
                        auto &entityAction = entityActionComponents[i];
                    } catch (std::exception &e) {
                        std::unique_ptr<Components::ActionComponent> actionComp = std::make_unique<Components::ActionComponent>();
                        engine.getRegistry().componentManager().addComponent<Components::ActionComponent>((ECS::Entity)i, std::move(actionComp));
                        std::cerr << "Set default action for entity: " << i << " in ActionManager." << std::endl;
                    };
                    auto &entityAction = entityActionComponents[i];

                    bool forwardInput = ctrl.inputs[(int)Action::FORWARD];
                    bool backwardInput = ctrl.inputs[(int)Action::BACKWARD];
                    bool rightInput = ctrl.inputs[(int)Action::RIGHT];
                    bool leftInput = ctrl.inputs[(int)Action::LEFT];
                    bool action1Input = ctrl.inputs[(int)Action::ACTION1];

                    EntityAction newAction = determinePressedAction(forwardInput, backwardInput, rightInput, leftInput, action1Input, action);
                    std::cerr << "New action initialized : " << newAction << std::endl;
                    entityAction->action = newAction;
                    return;
                }
                index++;
            }
        }
    }
    if (event.type == sf::Event::KeyReleased) {
        for (auto &&[i, ctrl] : IndexedZipper(ctrlComponents)) {
            Key key = Key::UNKNOWN;
            try {
                key = __sfmlToKey.at(event.key.code);
            } catch (std::exception &e) {
                std::cerr << "Key not handled" << std::endl;
            }
            int index = 0;
            for (auto &[action, actionKey] : ctrl.keyBindings) {
                if (actionKey == key) {
                    handleInputReleased(ctrl.inputs, ctrl.actions, index);
                    engine.updateComponent(i, ctrl.getId(), ctrl.serialize());
                    try {
                        auto &entityAction = entityActionComponents[i];
                    } catch (std::exception &e) {
                        std::unique_ptr<Components::ActionComponent> actionComp = std::make_unique<Components::ActionComponent>();
                        engine.getRegistry().componentManager().addComponent<Components::ActionComponent>((ECS::Entity)i, std::move(actionComp));
                        std::cerr << "Set default action for entity: " << i << " in ActionManager." << std::endl;
                    };

                    auto &entityAction = entityActionComponents[i];

                    bool forwardInput = ctrl.inputs[(int)Action::FORWARD];
                    bool backwardInput = ctrl.inputs[(int)Action::BACKWARD];
                    bool rightInput = ctrl.inputs[(int)Action::RIGHT];
                    bool leftInput = ctrl.inputs[(int)Action::LEFT];
                    bool action1Input = ctrl.inputs[(int)Action::ACTION1];

                    EntityAction newAction = determineReleasedAction(forwardInput, backwardInput, rightInput, leftInput, action1Input, action);
                    entityAction->action = newAction;
                    return;
                }
                index++;
            }
        }
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        for (auto &&[i, ctrl] : IndexedZipper(ctrlComponents)) {
            int index = 0;
            for (auto &[action, actionKey] : ctrl.keyBindings) {
                if ((actionKey == Key::LEFT_CLICK && event.mouseButton.button == sf::Mouse::Left) ||
                    (actionKey == Key::RIGHT_CLICK && event.mouseButton.button == sf::Mouse::Right) ||
                    (actionKey == Key::MIDDLE_CLICK && event.mouseButton.button == sf::Mouse::Middle)) {
                        handleInputPressed(ctrl.inputs, ctrl.actions, index);
                        engine.updateComponent(i, ctrl.getId(), ctrl.serialize());
                        try {
                            auto &entityAction = entityActionComponents[i];
                        } catch (std::exception &e) {
                            std::unique_ptr<Components::ActionComponent> actionComp = std::make_unique<Components::ActionComponent>();
                            engine.getRegistry().componentManager().addComponent<Components::ActionComponent>((ECS::Entity)i, std::move(actionComp));
                            std::cerr << "Set default action for entity: " << i << " in ActionManager." << std::endl;
                        };

                        auto &entityAction = entityActionComponents[i];
                            
                        bool forwardInput = ctrl.inputs[(int)Action::FORWARD];
                        bool backwardInput = ctrl.inputs[(int)Action::BACKWARD];
                        bool rightInput = ctrl.inputs[(int)Action::RIGHT];
                        bool leftInput = ctrl.inputs[(int)Action::LEFT];
                        bool action1Input = ctrl.inputs[(int)Action::ACTION1];

                        EntityAction newAction = determinePressedAction(forwardInput, backwardInput, rightInput, leftInput, action1Input, action);
                        entityAction->action = newAction;
                        return;
                }
                index++;
            }
        }
    }
    if (event.type == sf::Event::MouseButtonReleased) {
        for (auto &&[i, ctrl] : IndexedZipper(ctrlComponents)) {
            int index = 0;
            for (auto &[action, actionKey] : ctrl.keyBindings) {
                if ((actionKey == Key::LEFT_CLICK && event.mouseButton.button == sf::Mouse::Left) ||
                    (actionKey == Key::RIGHT_CLICK && event.mouseButton.button == sf::Mouse::Right) ||
                    (actionKey == Key::MIDDLE_CLICK && event.mouseButton.button == sf::Mouse::Middle)) {
                        handleInputReleased(ctrl.inputs, ctrl.actions, index);
                        engine.updateComponent(i, ctrl.getId(), ctrl.serialize());
                        try {
                            auto &entityAction = entityActionComponents[i];
                        } catch (std::exception &e) {
                            std::unique_ptr<Components::ActionComponent> actionComp = std::make_unique<Components::ActionComponent>();
                            engine.getRegistry().componentManager().addComponent<Components::ActionComponent>((ECS::Entity)i, std::move(actionComp));
                            std::cerr << "Set default action for entity: " << i << " in ActionManager." << std::endl;
                        };

                        auto &entityAction = entityActionComponents[i];

                        bool forwardInput = ctrl.inputs[(int)Action::FORWARD];
                        bool backwardInput = ctrl.inputs[(int)Action::BACKWARD];
                        bool rightInput = ctrl.inputs[(int)Action::RIGHT];
                        bool leftInput = ctrl.inputs[(int)Action::LEFT];
                        bool action1Input = ctrl.inputs[(int)Action::ACTION1];

                        EntityAction newAction = determineReleasedAction(forwardInput, backwardInput, rightInput, leftInput, action1Input, action);
                        entityAction->action = newAction;
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
