/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Raylib.cpp file
*/

#include "Raylib.hpp"
#include "library_entrypoint.hpp"
#include <raylib.h>

Systems::Raylib::Raylib() :
    displayManager(),
    inputManager()
{
}

Systems::Raylib::Raylib(libconfig::Setting &config) :
    displayManager(config),
    inputManager(config)
{
}

void Systems::Raylib::init(Engine::GameEngine &engine)
{
    displayManager.init(engine);
    inputManager.init(engine);
}

void Systems::Raylib::run(Engine::GameEngine &engine)
{
    if (WindowShouldClose()) {
        displayManager.freeAll(engine);
        exit(0);
    }
    inputManager.run(engine);
    displayManager.run(engine);
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryPoint()
{
    return new Systems::Raylib();
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryConfig(libconfig::Setting &config)
{
    return new Systems::Raylib(config);
}
