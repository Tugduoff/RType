/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Position.cpp file
*/

#include <memory>
#include "Position.hpp"

extern "C"
{
#ifdef _WIN32
    __declspec(dllexport)
#endif
    Components::IComponent *entryPoint(uint32_t x, uint32_t y)
    {
        return new Components::Position(x, y);
    }
}
