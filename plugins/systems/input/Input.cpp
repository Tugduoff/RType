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
#include "components/gun/Gun.hpp"
#include "components/position/Position.hpp"
#include "components/collider/Collider.hpp"
#include "library_entrypoint.hpp"
#include <iostream>
#include <stdexcept>

void shootAction(Engine::GameEngine &engine, size_t entityIndex) {
    auto &reg = engine.getRegistry();
    
    try {
        auto &gunComponents = reg.componentManager().getComponents<Components::Gun>();
        auto &gun = gunComponents[entityIndex];

        if (gun) {
            if (gun->chrono.getElapsedTime() >= gun->fireRate) {
                std::cerr << "Entity " << entityIndex << " fired a shot!" << std::endl;
                auto &positionComponents = reg.componentManager().getComponents<Components::Position>();
                auto &position = positionComponents[entityIndex];

                if (position) {
                    float projectilePosX = position->x;
                    float projectilePosY = position->y;
                    float projectileVelX = 5;
                    float projectileVelY = 0.0f;
                    float projectileColliderWidth = 10.0f;
                    float projectileColliderHeight = 10.0f;
                    int projectileDamage = gun ->damage;

                    Systems::InputSystem inputSystem;
                    inputSystem.createProjectile(engine, projectilePosX, projectilePosY, 
                        projectileVelX, projectileVelY, 
                        projectileColliderWidth, projectileColliderHeight, projectileDamage);
                } else {
                    std::cerr << "Position component missing for entity " << entityIndex << std::endl;
                }

                gun->chrono.restart();
            } else {
                std::cerr << "Gun is on cooldown." << std::endl;
            }
        }
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

std::unordered_map<int, std::function<void(Engine::GameEngine&, size_t)>> actionTable = {
    {1, shootAction},
    // Add more actions here
};

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
                    if (actionTable.find(j + 1) != actionTable.end()) {
                        actionTable[j + 1](engine, i);
                    } else {
                        std::cout << "No action mapped for Action " << j + 1 << std::endl;
                    }
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
    if (!engine.registerComponent<Components::Gun>("./plugins/bin/components/", "Gun"))
        std::cerr << "Error: Could not register Gun component in system Input" << std::endl;
    if (!engine.registerComponent<Components::Damage>("./plugins/bin/components/", "Damage"))
        std::cerr << "Error: Could not register Damage component in system Input" << std::endl;
    if (!engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position"))
        std::cerr << "Error: Could not register Position component in system Input" << std::endl;
    if (!engine.registerComponent<Components::Collider>("./plugins/bin/components/", "Collider"))
        std::cerr << "Error: Could not register Collider component in system Input" << std::endl;
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
