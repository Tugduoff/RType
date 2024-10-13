/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Damage.cpp file
*/

#include "Damage.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
char const *const componentName = "Damage";

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault() {
    return new Components::Damage();
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(uint32_t damage) {
    return new Components::Damage(damage);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config) {
    return new Components::Damage(config);
}
