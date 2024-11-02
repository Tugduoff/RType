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

            /**
             * @brief Handles looping behavior along the Y-axis for a given entity.
             * 
             * @param engine Reference to the game engine, which manages and updates the game state.
             * @param i Index of the entity within the engine's component storage.
             * @param pos Unique pointer to the Position component of the entity.
             * @param vel Unique pointer to the Velocity component of the entity.
             * @param deathRange Unique pointer to the DeathRange component, determining bounds for the entity.
             * @param factor Multiplier factor for movement calculations.
             */
            void yAxisLoop(Engine::GameEngine &engine, size_t i, std::unique_ptr<Components::Position> &pos,
                        std::unique_ptr<Components::Velocity> &vel, std::unique_ptr<Components::DeathRange> &deathRange, float factor);

            /**
             * @brief Handles looping behavior along the X-axis for a given entity.
             * 
             * @param engine Reference to the game engine, which manages and updates the game state.
             * @param i Index of the entity within the engine's component storage.
             * @param pos Unique pointer to the Position component of the entity.
             * @param vel Unique pointer to the Velocity component of the entity.
             * @param deathRange Unique pointer to the DeathRange component, determining bounds for the entity.
             * @param gun Unique pointer to the Gun component, which defines the entity's shooting behavior.
             * @param factor Multiplier factor for movement calculations.
             */
            void xAxisLoop(Engine::GameEngine &engine, size_t i, std::unique_ptr<Components::Position> &pos,
                        std::unique_ptr<Components::Velocity> &vel, std::unique_ptr<Components::DeathRange> &deathRange,
                        std::unique_ptr<Components::Gun> &gun, float factor);

            /**
             * @brief Creates a zigzag movement pattern along the Y-axis for a given entity within a specified range.
             * 
             * @param engine Reference to the game engine, which manages and updates the game state.
             * @param i Index of the entity within the engine's component storage.
             * @param pos Unique pointer to the Position component of the entity.
             * @param vel Unique pointer to the Velocity component of the entity.
             * @param deathRange Unique pointer to the DeathRange component, determining bounds for the entity.
             * @param factor Multiplier factor for movement calculations.
             * @param minValue Minimum Y value for the zigzag movement range.
             * @param maxValue Maximum Y value for the zigzag movement range.
             */
            void yZigZag(Engine::GameEngine &engine, size_t i, std::unique_ptr<Components::Position> &pos,
                        std::unique_ptr<Components::Velocity> &vel, std::unique_ptr<Components::DeathRange> &deathRange,
                        float factor, int minValue, int maxValue);

            /**
             * @brief Creates a zigzag movement pattern along the X-axis for a given entity within a specified range.
             * 
             * @param engine Reference to the game engine, which manages and updates the game state.
             * @param i Index of the entity within the engine's component storage.
             * @param pos Unique pointer to the Position component of the entity.
             * @param vel Unique pointer to the Velocity component of the entity.
             * @param deathRange Unique pointer to the DeathRange component, determining bounds for the entity.
             * @param gun Unique pointer to the Gun component, which defines the entity's shooting behavior.
             * @param factor Multiplier factor for movement calculations.
             * @param minValue Minimum X value for the zigzag movement range.
             * @param maxValue Maximum X value for the zigzag movement range.
             */
            void xZigZag(Engine::GameEngine &engine, size_t i, std::unique_ptr<Components::Position> &pos,
                        std::unique_ptr<Components::Velocity> &vel, std::unique_ptr<Components::DeathRange> &deathRange,
                        std::unique_ptr<Components::Gun> &gun, float factor, int minValue, int maxValue);

        private:
    };
};

#endif // BEHAVIOR_HPP
