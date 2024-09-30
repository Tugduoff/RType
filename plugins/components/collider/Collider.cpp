/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Collider.cpp file
*/

#include <memory>
#include "Collider.hpp"

extern "C" std::unique_ptr<Components::IComponent> entryPoint(uint8_t shape, uint32_t radius, uint32_t width, uint32_t height)
{
    return std::make_unique<Components::Collider>(shape, radius, width, height);
}
