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
Components::IComponent *buildWithParams(std::string sound, uint8_t volume, uint8_t pitch, uint8_t loop)
{
    return new Components::Sound(sound, volume, pitch, loop);
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::Sound(config);
}
