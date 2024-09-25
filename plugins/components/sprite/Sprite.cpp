/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Sprite.cpp file
*/

#include "Sprite.hpp"

extern "C" IComponent *entryPoint(uint32_t texture_id)
{
    return new Sprite(texture_id);
}