/*
** EPITECH PROJECT, 2024
** RType
** File description:
** InputSystem.hpp file
*/

#ifndef INPUTSYSTEM_HPP
    #define INPUTSYSTEM_HPP

    #include <libconfig.h++>
    #include "systems/ASystem.hpp"
    #include "components/velocity/Velocity.hpp"
    #include "components/position/Position.hpp"
    #include "components/collider/Collider.hpp"
    #include "components/damage/Damage.hpp"

namespace Engine {
    class GameEngine;
}

namespace Systems {
    /**
     * @class InputSystem
     * 
     * @brief System class from Systems that handles input events.
     * 
     * This system is responsible handling input events and updating entities based on the input.
     */
    class InputSystem : public ASystem {
        public:
            InputSystem() = default;
            InputSystem(libconfig::Setting &config);
            ~InputSystem() = default;

            void run(Engine::GameEngine &engine) override;
            void init(Engine::GameEngine &engine) override;
            void createProjectile(Engine::GameEngine &engine, 
                float posX, float posY, 
                float velX, float velY, 
                float colliderWidth,
                float colliderHeight, 
                int damageValue) 
            {
                auto &reg = engine.getRegistry();
                ECS::Entity projectile = reg.createEntity();

                std::unique_ptr<Components::Position> positionComponent =
                    std::make_unique<Components::Position>(posX, posY, 1);
                reg.componentManager().addComponent(projectile, std::move(positionComponent));
                std::unique_ptr<Components::Velocity> velocityComponent =
                    std::make_unique<Components::Velocity>(velX, velY, 100);
                reg.componentManager().addComponent(projectile, std::move(velocityComponent));
                std::unique_ptr<Components::Collider> colliderComponent =
                    std::make_unique<Components::Collider>(colliderWidth, colliderHeight);
                reg.componentManager().addComponent(projectile, std::move(colliderComponent));
                std::unique_ptr<Components::Damage> damageComponent =
                    std::make_unique<Components::Damage>(damageValue);
                reg.componentManager().addComponent(projectile, std::move(damageComponent));

                return;
            }

        private:
    };
};

#endif // INPUTSYSTEM_HPP
