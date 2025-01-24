/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Spawn.hpp file
*/

#ifndef SPAWN_LFB_HPP
    #define SPAWN_LFB_HPP

    #include <libconfig.h++>
    #include "systems/ASystem.hpp"

namespace Systems {
    /**
     * @class Spawn
     * 
     * @brief System class from Systems that handles the Raylib window and input.
     * 
     * This system is responsible for creating the Raylib window and handling input events.
     * Thus, it is responsible for running the Spawn and Input systems.
     */
    class Spawn : public ASystem {
        public:

            Spawn();
            Spawn(libconfig::Setting &config);
            ~Spawn() = default;

            void run(Engine::GameEngine &engine) override;
            void init(Engine::GameEngine &engine) override;

        private:

            void spawnLeafAtRandomPosition(Engine::GameEngine &engine);

            uint32_t __speed;
            uint32_t __maxLeaves;
            uint32_t __width;
            uint32_t __height;
    };
};

#endif // SPAWN_LFB_HPP
