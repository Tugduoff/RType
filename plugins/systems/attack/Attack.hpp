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
                const std::string &spriteID
            );
    };
};

#endif // ATTACKSYSTEM_HPP
