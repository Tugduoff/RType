/*
** EPITECH PROJECT, 2024
** RType
** File description:
** MoveSystem.hpp file
*/

#ifndef MOVESYSTEM_HPP
    #define MOVESYSTEM_HPP

    #include "plugins/systems/ASystem.hpp"

namespace Systems {
    class MoveSystem : public ASystem {
        public:
            MoveSystem() = default;
            ~MoveSystem() = default;

            void func(ECS::Registry &reg) override;
        private:
    };
};

#endif // MOVESYSTEM_HPP