/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Gun.cpp file
*/

#include "Gun.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
char const *const componentName = "Gun";

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault() {
    return new Components::Gun();
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(uint32_t bulletDamage, uint32_t fireRate, uint32_t bulletVelocityX, uint32_t bulletVelocityY, std::string spriteId) {
    return new Components::Gun(bulletDamage, fireRate, bulletVelocityX, bulletVelocityY, spriteId);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config) {
    return new Components::Gun(config);
}
