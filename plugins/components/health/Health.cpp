/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Health.cpp file
*/

#include "Health.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
char const *const componentName = "Health";

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault()
{
    return new Components::Health();
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(uint32_t currentHealth, uint32_t maxHealth)
{
    return new Components::Health(currentHealth, maxHealth);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Health(config);
}
