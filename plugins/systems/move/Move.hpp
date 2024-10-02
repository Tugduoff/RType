/*
** EPITECH PROJECT, 2024
** RType
** File description:
** MoveSystem.hpp file
*/

#ifndef MOVESYSTEM_HPP
    #define MOVESYSTEM_HPP

    #include "systems/ASystem.hpp"

namespace Systems {
    class MoveSystem : public ASystem {
        public:
            MoveSystem() = default;
            ~MoveSystem() = default;

            void run(ECS::Registry &registry) override;
            void init(const ECS::ComponentRegisterer &registerer) override;
        private:
    };
};

#endif // MOVESYSTEM_HPP
