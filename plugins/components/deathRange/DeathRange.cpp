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
Components::IComponent *buildWithParams(uint32_t maxX, uint32_t maxY, uint32_t minX, uint32_t minY) {
    return new Components::DeathRange(maxX, maxY, minX, minY);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config) {
    return new Components::DeathRange(config);
}
