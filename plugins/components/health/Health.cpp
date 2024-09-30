/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Health.cpp file
*/

#include <memory>
#include "Health.hpp"

extern "C" std::unique_ptr<Components::IComponent> entryPoint(uint32_t currentHealth, uint32_t maxHealth)
{
    return std::make_unique<Components::Health>(currentHealth, maxHealth);
}
