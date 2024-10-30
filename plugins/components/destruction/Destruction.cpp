/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Destruction.cpp file
*/

#include "Destruction.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
char const *const componentName = "Destruction";

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault()
{
    return new Components::Destruction();
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(uint32_t duration, std::string id, std::tuple<std::string, uint8_t, uint8_t, uint8_t> sound, std::pair<uint32_t, uint32_t> scale)
{
    return new Components::Destruction(duration, id, sound, scale);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Destruction(config);
}
