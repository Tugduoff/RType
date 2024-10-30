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
    #include "components/gun/Gun.hpp"
    #include "components/Ai/Ai.hpp"

namespace Engine {
    class GameEngine;
}

namespace Systems {
    /**
     * @class BehaviorSystem
     * 
     * @brief System class from Systems that handles the movement and attacks of non controllable entities.
     * 
     * This system is responsible for handling the movement and attacks of non playable entities.
     */
    class BehaviorSystem : public ASystem {
        public:
            BehaviorSystem() = default;
            BehaviorSystem(libconfig::Setting &config);
            ~BehaviorSystem() = default;

            void run(Engine::GameEngine &engine) override;
            void init(Engine::GameEngine &engine) override;

            void yAxisLoop(Engine::GameEngine &engine, size_t i,
                std::unique_ptr<Components::Position> &pos, std::unique_ptr<Components::Velocity> &vel,
                std::unique_ptr<Components::DeathRange> &deathRange, float factor);
            void xAxisLoop(Engine::GameEngine &engine, size_t i, std::unique_ptr<Components::Position> &pos,
                std::unique_ptr<Components::Velocity> &vel, std::unique_ptr<Components::DeathRange> &deathRange,
                std::unique_ptr<Components::Gun> &gun, float factor);
            void yZigZag(Engine::GameEngine &engine, size_t i, std::unique_ptr<Components::Position> &pos,
                std::unique_ptr<Components::Velocity> &vel, std::unique_ptr<Components::DeathRange> &deathRange,
                float factor, int minValue, int maxValue);
            void xZigZag(void);
        private:
    };
};

#endif // BEHAVIOR_HPP
