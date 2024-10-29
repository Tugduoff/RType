/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Projectile.hpp file
*/

#ifndef PROJECTILE_HPP
    #define PROJECTILE_HPP

    #include "GameEngine/GameEngine.hpp"
    #include "components/type/Type.hpp"

void createProjectile(
    Engine::GameEngine &engine, 
    int posX, int posY, 
    int velX, int velY, 
    int colliderWidth,
    int colliderHeight, 
    int damageValue,
    enum Components::TypeID type,
    const std::string &spriteID
);

#endif // PROJECTILE_HPP
