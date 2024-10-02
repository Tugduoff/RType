/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Collider.cpp file
*/

#include <memory>
#include "Collider.hpp"

extern "C" std::unique_ptr<Components::IComponent> entryPoint(uint32_t shape, uint32_t radius, uint32_t width, uint32_t height)
{
    if (shape == 0) {
        return std::make_unique<Components::Collider>(static_cast<float>(radius));
    } else if (shape == 1) {
        return std::make_unique<Components::Collider>(static_cast<float>(width), static_cast<float>(height));
    }
    throw std::invalid_argument("Invalid shape type provided");
}

