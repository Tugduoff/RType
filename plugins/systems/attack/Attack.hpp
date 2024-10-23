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
    };
};

#endif // ATTACKSYSTEM_HPP
