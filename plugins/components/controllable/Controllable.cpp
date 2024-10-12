/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Controllable.cpp file
*/

#include "Controllable.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
Components::IComponent *entryPoint(std::unordered_map<Action, Key> keyBindings = {
    {Action::FORWARD, Key::Z},
    {Action::BACKWARD, Key::S},
    {Action::LEFT, Key::Q},
    {Action::RIGHT, Key::D},
    {Action::ACTION1, Key::LEFT_CLICK},
    {Action::ACTION2, Key::RIGHT_CLICK},
    {Action::ACTION3, Key::MIDDLE_CLICK},
    {Action::ACTION4, Key::NUM_0},
    {Action::ACTION5, Key::NUM_1},
    {Action::ACTION6, Key::NUM_2},
    {Action::ACTION7, Key::NUM_3},
    {Action::ACTION8, Key::NUM_4},
    {Action::ACTION9, Key::NUM_5},
    {Action::ACTION10, Key::NUM_6}
})
{
    return new Components::Controllable(keyBindings);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Controllable(config);
}

LIBRARY_ENTRYPOINT
char const *const componentName = "Controllable";
