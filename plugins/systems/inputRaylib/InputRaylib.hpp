/*
** EPITECH PROJECT, 2024
** RType
** File description:
** InputRaylibSystem.hpp file
*/

#ifndef INPUT_SYSTEM_HPP
    #define INPUT_SYSTEM_HPP

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

    #define DEG_TO_RAD 0.017453292519943295769236907684886

namespace Engine {
    class GameEngine;
}

namespace Systems {
    /**
     * @class InputRaylibSystem
     * 
     * @brief System class from Systems that handles input events.
     * 
     * This system is responsible handling input events and updating entities based on the input.
     */
    class InputRaylibSystem : public ASystem {
        public:
            InputRaylibSystem() = default;
            InputRaylibSystem(libconfig::Setting &config);
            ~InputRaylibSystem() = default;

            void run(Engine::GameEngine &engine) override;
            void init(Engine::GameEngine &engine) override;

        private:

    };
};

struct Vector3 {
    float x;
    float y;
    float z;
};

#endif // INPUT_SYSTEM_HPP
