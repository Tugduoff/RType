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
    #include "components/spriteId/SpriteID.hpp"
    #include "components/deathRange/DeathRange.hpp"
    #include "components/type/Type.hpp"
    #include <unordered_map>

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
            

        private:

            void shootAction(Engine::GameEngine &engine, size_t entityIndex);

    };
};

#endif // INPUTSYSTEM_HPP
