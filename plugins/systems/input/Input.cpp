/*
** EPITECH PROJECT, 2024
** RType
** File description:
** InputSystem.cpp file
*/

#include "GameEngine/GameEngine.hpp"
#include "Input.hpp"
#include "Actions.hpp"
#include "components/acceleration/Acceleration.hpp"
#include "components/velocity/Velocity.hpp"
#include "components/controllable/Controllable.hpp"
#include "library_entrypoint.hpp"
#include <iostream>
#include <stdexcept>

Systems::InputSystem::InputSystem(libconfig::Setting &)
{
}

void Systems::InputSystem::run(Engine::GameEngine &engine)
{
    auto &reg = engine.getRegistry();

    try {
        auto &controllableComponents = reg.componentManager().getComponents<Components::Controllable>();
        auto &accelerationComponents = reg.componentManager().getComponents<Components::Acceleration>();
        auto &velocityComponents = reg.componentManager().getComponents<Components::Velocity>();

        size_t i = 0;
        for (i = 0;
            i < controllableComponents.size() &&
            i < accelerationComponents.size() &&
            i < velocityComponents.size(); i++) {
            auto &controllable = controllableComponents[i];
            auto &acceleration = accelerationComponents[i];
            auto &velocity = velocityComponents[i];

            if (!controllable || !acceleration || !velocity)
                continue;

            if (controllable->inputs[(int)Action::FORWARD]) {
                velocity->x = acceleration->forward;
                std::cout << "Forward triggered" << std::endl;
                engine.updateComponent(i, velocity->getId(), velocity->serialize());
            }
            if (controllable->inputs[(int)Action::BACKWARD]) {
                velocity->x = acceleration->backward;
                std::cout << "Backward triggered" << std::endl;
                engine.updateComponent(i, velocity->getId(), velocity->serialize());
            }
            if (controllable->inputs[(int)Action::RIGHT]) {
                velocity->y = acceleration->right;
                std::cout << "Right triggered" << std::endl;
                engine.updateComponent(i, velocity->getId(), velocity->serialize());
            }
            if (controllable->inputs[(int)Action::LEFT]) {
                velocity->y = acceleration->left;
                std::cout << "Left triggered" << std::endl;
                engine.updateComponent(i, velocity->getId(), velocity->serialize());
            }
            for (int j = 0; j < 10; j++) {
                if (controllable->actions[j]) {
                    std::cout << "Action " << j + 1 << " triggered" << std::endl;
                }
            }
        }
    } catch (std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void Systems::InputSystem::init(Engine::GameEngine &engine)
{
    if (!engine.registerComponent<Components::Acceleration>("./plugins/bin/components/", "Acceleration"))
        std::cerr << "Error: Could not register Acceleration component in system Input" << std::endl;
    if (!engine.registerComponent<Components::Controllable>("./plugins/bin/components/", "Controllable"))
        std::cerr << "Error: Could not register Controllable component in system Input" << std::endl;
    if (!engine.registerComponent<Components::Velocity>("./plugins/bin/components/", "Velocity"))
        std::cerr << "Error: Could not register Velocity component in system Input" << std::endl;
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryPoint()
{
    return new Systems::InputSystem();
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryConfig(libconfig::Setting &config)
{
    return new Systems::InputSystem(config);
}
