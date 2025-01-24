/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Move.cpp file
*/

#include "components/position/Position.hpp"
#include "components/type/Type.hpp"
#include "components/rotation/Rotation.hpp"
#include "ECS/utilities/Zipper/Zipper.hpp"
#include "ECS/utilities/Zipper/IndexedZipper.hpp"
#include "Move.hpp"
#include "library_entrypoint.hpp"
#include <cmath>

Systems::Move::Move()
{
    __speed = 1;
}

Systems::Move::Move(libconfig::Setting &config)
{
    unsigned int speed = config.lookup("speed");

    __speed = speed;
}

void Systems::Move::init(Engine::GameEngine &engine)
{
    engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position");
    engine.registerComponent<Components::Type>("./plugins/bin/components/", "Type");
    engine.registerComponent<Components::Rotation>("./plugins/bin/components/", "Rotation");
}

void Systems::Move::run(Engine::GameEngine &engine)
{
    auto &componentManager = engine.getRegistry().componentManager();
    auto &posComponentArr = componentManager.getComponents<Components::Position>();
    auto &typeComponentArr = componentManager.getComponents<Components::Type>();
    auto &rotComponentArr = componentManager.getComponents<Components::Rotation>();

    int playerIndex = -1;

    for (auto &&[i, type] : IndexedZipper(typeComponentArr)) {
        if (type.id == Components::TypeID::ALLY) {
            playerIndex = i;
        }
    }

    if (playerIndex == -1) {
        std::cerr << "No player found" << std::endl;
        return;
    }

    // check where the cursor is and move the player to the cursor
    Vector2 cursor = GetMousePosition();

    try {
        auto &pos = posComponentArr[playerIndex];
        auto &rot = rotComponentArr[playerIndex];

        // Smooth movement using linear interpolation (lerp)
        const float lerpFactor = 0.1f;
        pos->x += (cursor.x - pos->x) * lerpFactor * __speed / 10;
        pos->y += (cursor.y - pos->y) * lerpFactor * __speed / 10;

        // Rotate the player towards the cursor
        float angle = atan2(cursor.y - pos->y, cursor.x - pos->x) * RAD2DEG;
        rot->z = angle;

        DrawCircle(pos->x, pos->y, 80, DARKGREEN);
        // DrawCircle(cursor.x, cursor.y, 40, DARKGREEN);

    } catch (std::out_of_range &e) {
        std::cerr << "Player position not found" << std::endl;
    }
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryPoint()
{
    return new Systems::Move();
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryConfig(libconfig::Setting &config)
{
    return new Systems::Move(config);
}
