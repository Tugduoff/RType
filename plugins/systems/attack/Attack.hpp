/*
** EPITECH PROJECT, 2024
** RType
** File description:
** AttackSystem.hpp file
*/

#ifndef ATTACKSYSTEM_HPP
    #define ATTACKSYSTEM_HPP

    #include <libconfig.h++>
    #include "systems/ASystem.hpp"
    #include "components/velocity/Velocity.hpp"
    #include "components/position/Position.hpp"
    #include "components/collider/Collider.hpp"
    #include "components/damage/Damage.hpp"
    #include "components/type/Type.hpp"
    #include "components/spriteId/SpriteID.hpp"
    #include "components/deathRange/DeathRange.hpp"

namespace Engine {
    class GameEngine;
}

namespace Systems {
    /**
     * @class AttackSystem
     * 
     * @brief System class from Systems that handles the attack of entities (automatic attacks).
     * 
     * This system is responsible for handling the attack of entities.
     */
    class AttackSystem : public ASystem {
        public:
            AttackSystem() = default;
            AttackSystem(libconfig::Setting &config);
            ~AttackSystem() = default;

            void run(Engine::GameEngine &engine) override;
            void init(Engine::GameEngine &engine) override;
        private:

            void createProjectile(Engine::GameEngine &engine, 
                int posX, int posY, 
                int velX, int velY, 
                int colliderWidth,
                int colliderHeight, 
                int damageValue,
                enum Components::TypeID type,
                const std::string &spriteID)
            {
                auto &reg = engine.getRegistry();
                ECS::Entity projectile = reg.createEntity();

                std::cerr << "Created new projectile with id: " << projectile << std::endl;

                std::unique_ptr<Components::Position> positionComponent =
                    std::make_unique<Components::Position>(posX, posY, 1);
                Components::IComponent *rawPos = positionComponent.get();
                reg.componentManager().addComponent(projectile, std::move(positionComponent));
                std::unique_ptr<Components::Velocity> velocityComponent =
                    std::make_unique<Components::Velocity>(velX, velY, 100);
                Components::IComponent *rawVel = velocityComponent.get();
                reg.componentManager().addComponent(projectile, std::move(velocityComponent));
                std::unique_ptr<Components::Collider> colliderComponent =
                    std::make_unique<Components::Collider>(colliderWidth, colliderHeight);
                Components::IComponent *rawCollider = colliderComponent.get();
                reg.componentManager().addComponent(projectile, std::move(colliderComponent));
                std::unique_ptr<Components::Damage> damageComponent =
                    std::make_unique<Components::Damage>(damageValue);
                Components::IComponent *rawDamage = damageComponent.get();
                reg.componentManager().addComponent(projectile, std::move(damageComponent));
                std::unique_ptr<Components::Type> typeComponent =
                    std::make_unique<Components::Type>(type);
                Components::IComponent *rawType = typeComponent.get();
                reg.componentManager().addComponent(projectile, std::move(typeComponent));
                std::unique_ptr<Components::SpriteID> spriteIDComponent =
                    std::make_unique<Components::SpriteID>(spriteID);
                Components::IComponent *rawSpriteID = spriteIDComponent.get();
                reg.componentManager().addComponent(projectile, std::move(spriteIDComponent));
                std::unique_ptr<Components::DeathRange> deathRangeComponent =
                    std::make_unique<Components::DeathRange>(1920, 1080, 0, 0);
                Components::IComponent *rawDeathRange = deathRangeComponent.get();
                reg.componentManager().addComponent(projectile, std::move(deathRangeComponent));

                engine.updateComponent(projectile, rawPos->getId(), rawPos->serialize());
                engine.updateComponent(projectile, rawVel->getId(), rawVel->serialize());
                engine.updateComponent(projectile, rawCollider->getId(), rawCollider->serialize());
                engine.updateComponent(projectile, rawDamage->getId(), rawDamage->serialize());
                engine.updateComponent(projectile, rawType->getId(), rawType->serialize());
                engine.updateComponent(projectile, rawSpriteID->getId(), rawSpriteID->serialize());
                engine.updateComponent(projectile, rawDeathRange->getId(), rawDeathRange->serialize());
                return;
            }
    };
};

#endif // ATTACKSYSTEM_HPP
