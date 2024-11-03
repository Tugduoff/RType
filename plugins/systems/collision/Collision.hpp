/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Collision.hpp file
*/

#ifndef COLLISION_HPP
    #define COLLISION_HPP

    #include <libconfig.h++>
    #include "systems/ASystem.hpp"
    #include "ECS/entity/Entity.hpp"
    #include "utils/Chrono.hpp"

namespace Engine {
    class GameEngine;
}

namespace Systems {
    /**
     * @class Collision
     * 
     * @brief System class from Systems that handles the collision.
     * 
     * This system is responsible for handling the collision between entities.
     */
    class Collision : public ASystem {
        public:
            Collision() = default;
            Collision(libconfig::Setting &config);
            ~Collision() = default;

            void run(Engine::GameEngine &engine) override;
            void init(Engine::GameEngine &engine) override;
        private:

            /**
             * @brief Check the collision between player projectiles and enemies.
             * 
             * @param reg The registry of the game engine
             * 
             * @note This function will check the collision between player projectiles and enemies.
             */
            void checkPlayerProjectileToEnemyCollision(Engine::GameEngine &engine);

            /**
             * @brief Check the collision between enemy projectiles and the player.
             * 
             * @param reg The registry of the game engine
             * 
             * @note This function will check the collision between enemy projectiles and the player.
             */
            void checkEnemyProjectileToPlayerCollision(Engine::GameEngine &engine);

            /**
             * @brief Spawn the death effect of an entity.
             * 
             * @param engine The game engine
             * @param entity The entity to spawn the death effect for
             * 
             * @note This function will spawn the death effect of an entity.
             */
            void spawnDeathEffect(Engine::GameEngine &engine, ECS::Entity entity);

            void checkChronos(Engine::GameEngine &engine);

            std::vector<std::tuple<ECS::Entity, Chrono, unsigned int>> __deathEffectsCooldown;
    };
};

#endif // COLLISION_HPP
