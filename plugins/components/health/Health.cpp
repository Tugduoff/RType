/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Health.cpp file
*/

#include "Health.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
Components::IComponent *entryPoint(uint32_t currentHealth, uint32_t maxHealth)
{
    return new Components::Health(currentHealth, maxHealth);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Health(config);
}

LIBRARY_ENTRYPOINT
char const *componentName = "Health";

Components::Health::Health(libconfig::Setting &config)
{
    config.lookupValue("currentHealth", currentHealth);
    config.lookupValue("maxHealth", maxHealth);
}
