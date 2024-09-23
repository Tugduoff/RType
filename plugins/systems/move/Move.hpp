/*
** EPITECH PROJECT, 2024
** RType
** File description:
** MoveSystem.hpp file
*/

#ifndef MOVESYSTEM_HPP
    #define MOVESYSTEM_HPP

    #include "ASystem.hpp"
    #include "Registry.hpp"

class MoveSystem : public ASystem {
    public:
        MoveSystem() = default;
        ~MoveSystem() = default;

        void func(Registry &reg) override;
    private:
};

#endif // MOVESYSTEM_HPP
