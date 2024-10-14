/*
** EPITECH PROJECT, 2024
** RType
** File description:
** DeathRange.cpp file
*/

#include "DeathRange.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
char const *const componentName = "DeathRange";

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault() {
    return new Components::DeathRange();
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(uint32_t maximum, uint32_t minimum) {
    return new Components::DeathRange(maximum, minimum);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config) {
    return new Components::DeathRange(config);
}
