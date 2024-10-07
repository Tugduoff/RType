/*
** EPITECH PROJECT, 2024
** RType
** File description:
** MoveSystem.hpp file
*/

#ifndef MOVESYSTEM_HPP
    #define MOVESYSTEM_HPP

    #include <libconfig.h++>
    #include "systems/AServerSystem.hpp"

namespace Systems {
    class MoveSystem : public AServerSystem {
        public:
            MoveSystem() = default;
            MoveSystem(libconfig::Setting &config);
            ~MoveSystem() = default;

            void run(Engine::ServerEngine &engine) override;
            void init(Engine::ServerEngine &engine) override;
        private:
    };
};

#endif // MOVESYSTEM_HPP
