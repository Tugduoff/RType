/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Visible.cpp file
*/

#include <memory>
#include "Visible.hpp"

extern "C" std::unique_ptr<Components::IComponent> entryPoint(bool isVisible)
{
    return std::make_unique<Components::Visible>(isVisible);
}
