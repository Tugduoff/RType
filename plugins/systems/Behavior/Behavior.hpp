/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Behavior.hpp file
*/

#ifndef BEHAVIOR_HPP
    #define BEHAVIOR_HPP

    #include <libconfig.h++>
    #include "systems/ASystem.hpp"
    #include "components/velocity/Velocity.hpp"
    #include "components/position/Position.hpp"
    #include "components/collider/Collider.hpp"
    #include "components/damage/Damage.hpp"
    #include "components/type/Type.hpp"
    #include "components/spriteId/SpriteID.hpp"
    #include "components/deathRange/DeathRange.hpp"
    #include "components/Ai/Ai.hpp"

namespace Engine {
    class GameEngine;
}

namespace Systems {
    /**
     * @class AiSystem
     * 
     * @brief System class from Systems that handles the attack of entities (automatic attacks).
     * 
     * This system is responsible for handling the attack of entities.
     */
    class BehaviorSystem : public ASystem {
        public:
            BehaviorSystem() = default;
            BehaviorSystem(libconfig::Setting &config);
            ~BehaviorSystem() = default;

            void run(Engine::GameEngine &engine) override;
            void init(Engine::GameEngine &engine) override;
        private:
    };
};

#endif // BEHAVIOR_HPP
