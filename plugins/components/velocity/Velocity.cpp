/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Velocity.cpp file
*/

#include <memory>
#include "Velocity.hpp"

extern "C"
{
#ifdef _WIN32
    __declspec(dllexport) Components::IComponent *entryPoint(uint32_t x, uint32_t y)
#else
    Components::IComponent *entryPoint(uint32_t x, uint32_t y)
#endif
    {
        return new Components::Velocity(x, y);
    }
}
