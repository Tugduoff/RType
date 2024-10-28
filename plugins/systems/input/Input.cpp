/*
** EPITECH PROJECT, 2024
** RType
** File description:
** InputSystem.cpp file
*/

#include "GameEngine/GameEngine.hpp"
#include "Input.hpp"
#include "utils/Actions.hpp"
#include "components/acceleration/Acceleration.hpp"
#include "components/velocity/Velocity.hpp"
#include "components/controllable/Controllable.hpp"
#include "components/gun/Gun.hpp"
#include "components/position/Position.hpp"
#include "components/collider/Collider.hpp"
#include "library_entrypoint.hpp"
#include "utils/Projectile.hpp"
#include <iostream>
#include <stdexcept>
#include <unordered_map>

void Systems::InputSystem::shootAction(Engine::GameEngine &engine, size_t entityIndex) {
    auto &reg = engine.getRegistry();
    
    try {
        auto &gunComponents = reg.componentManager().getComponents<Components::Gun>();
        auto &gun = gunComponents[entityIndex];

        if (gun) {
            if (gun->chrono.getElapsedTime() >= gun->fireRate) {
                std::cerr << "Entity " << entityIndex << " fired a shot!" << std::endl;
                gun->chrono.restart();
                auto &positionComponents = reg.componentManager().getComponents<Components::Position>();
                auto &position = positionComponents[entityIndex];

                int projectilePosX = position->x;
                int projectilePosY = position->y;
                int projectileVelX = gun->bulletVelocity;
                int projectileVelY = 0;
                int projectileColliderWidth = 10;
                int projectileColliderHeight = 10;
                int projectileDamage = gun->bulletDamage;
                enum Components::TypeID type = Components::TypeID::ALLY_PROJECTILE;
                std::string spriteId = gun->spriteId;

                createProjectile(engine, projectilePosX, projectilePosY, 
                    projectileVelX, projectileVelY, 
                    projectileColliderWidth, projectileColliderHeight, projectileDamage, type, spriteId);

            } else {
                std::cerr << "Gun is on cooldown." << std::endl;
            }
        }
    } catch (std::exception &e) {
        std::cerr << "Input Error: " << e.what() << std::endl;
    }
}

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
            try {
                auto &controllable = controllableComponents[i];
                auto &acceleration = accelerationComponents[i];
                auto &velocity = velocityComponents[i];

                bool inputForward = controllable->inputs[(int)Action::FORWARD];
                bool inputBackward = controllable->inputs[(int)Action::BACKWARD];
                bool inputRight = controllable->inputs[(int)Action::RIGHT];
                bool inputLeft = controllable->inputs[(int)Action::LEFT];

                // All the "engine.updateComponent" are commented because we had
                // trouble with the packet loss and removed all not used 
                // packets by the clients. 

                if (inputForward && !inputBackward) {
                    velocity->x = acceleration->forward;
                    std::cout << "Forward triggered" << std::endl;
                    // engine.updateComponent(i, velocity->getId(), velocity->serialize());
                }
                if (inputBackward && !inputForward) {
                    velocity->x = acceleration->backward;
                    std::cout << "Backward triggered" << std::endl;
                    // engine.updateComponent(i, velocity->getId(), velocity->serialize());
                }
                if (inputRight && !inputLeft) {
                    velocity->y = acceleration->right;
                    std::cout << "Right triggered" << std::endl;
                    // engine.updateComponent(i, velocity->getId(), velocity->serialize());
                }
                if (inputLeft && !inputRight) {
                    velocity->y = acceleration->left;
                    std::cout << "Left triggered" << std::endl;
                    // engine.updateComponent(i, velocity->getId(), velocity->serialize());
                }
                for (int j = 0; j < 10; j++) {
                    if (controllable->actions[j]) {
                        std::cout << "Action " << j + 1 << " triggered" << std::endl;
                        if (j == 0) {
                            shootAction(engine, i);
                        } else {
                            std::cout << "No action mapped for Action " << j + 1 << std::endl;
                        }
                    }
                }
            } catch (std::exception &e) {
                continue;
            }
        }
    } catch (std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void Systems::InputSystem::init(Engine::GameEngine &engine)
{
    engine.registerComponent<Components::Acceleration>("./plugins/bin/components/", "Acceleration");
    engine.registerComponent<Components::Controllable>("./plugins/bin/components/", "Controllable");
    engine.registerComponent<Components::Velocity>("./plugins/bin/components/", "Velocity");
    engine.registerComponent<Components::Gun>("./plugins/bin/components/", "Gun");
    engine.registerComponent<Components::Damage>("./plugins/bin/components/", "Damage");
    engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position");
    engine.registerComponent<Components::Collider>("./plugins/bin/components/", "Collider");
    engine.registerComponent<Components::SpriteID>("./plugins/bin/components/", "SpriteID");
    engine.registerComponent<Components::Type>("./plugins/bin/components/", "Type");
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
