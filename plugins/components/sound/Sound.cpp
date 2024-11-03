/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Sound.cpp file
*/

#include "Sound.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
char const *const componentName = "Sound";

LIBRARY_ENTRYPOINT
Components::IComponent *buildDefault()
{
    return new Components::Sound();
}

LIBRARY_ENTRYPOINT
Components::IComponent *buildWithParams(std::vector<std::tuple<std::string, std::string, uint8_t, uint8_t, bool, bool>> sounds)
{
    return new Components::Sound(sounds);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Sound(config);
}
