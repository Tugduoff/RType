/*
** EPITECH PROJECT, 2024
** RType
** File description:
** InputSystem.cpp file
*/

#include "ECS/utilities/Zipper/IndexedZipper.hpp"
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
        auto &controllableArr = reg.componentManager().getComponents<Components::Controllable>();
        auto &accelArr = reg.componentManager().getComponents<Components::Acceleration>();
        auto &velArr = reg.componentManager().getComponents<Components::Velocity>();

        for (auto &&[i, controllable, accel, vel] : IndexedZipper(controllableArr, accelArr, velArr)) {
            bool inputForward = controllable.inputs[(int)Action::FORWARD];
            bool inputBackward = controllable.inputs[(int)Action::BACKWARD];
            bool inputRight = controllable.inputs[(int)Action::RIGHT];
            bool inputLeft = controllable.inputs[(int)Action::LEFT];

            if (inputForward && !inputBackward) {
                vel.x = accel.forward;
                std::cout << "Forward triggered" << std::endl;
                engine.updateComponent(i, vel.getId(), vel.serialize());
            }
            if (inputBackward && !inputForward) {
                vel.x = accel.backward;
                std::cout << "Backward triggered" << std::endl;
                engine.updateComponent(i, vel.getId(), vel.serialize());
            }
            if (inputRight && !inputLeft) {
                vel.y = accel.right;
                std::cout << "Right triggered" << std::endl;
                engine.updateComponent(i, vel.getId(), vel.serialize());
            }
            if (inputLeft && !inputRight) {
                vel.y = accel.left;
                std::cout << "Left triggered" << std::endl;
                engine.updateComponent(i, vel.getId(), vel.serialize());
            }
            for (int j = 0; j < 10; j++) {
                if (controllable.actions[j]) {
                    std::cout << "Action " << j + 1 << " triggered" << std::endl;
                    if (j == 0) {
                        shootAction(engine, i);
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
    if (!engine.registerComponent<Components::SpriteID>("./plugins/bin/components/", "SpriteID"))
        std::cerr << "Error: Could not register SpriteID component in system Input" << std::endl;
    if (!engine.registerComponent<Components::Type>("./plugins/bin/components/", "Type"))
        std::cerr << "Error: Could not register Type component in system Input" << std::endl;
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
