/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Display.cpp file
*/

#include "components/position/Position.hpp"
#include "components/type/Type.hpp"
#include "components/rotation/Rotation.hpp"
#include "components/score/Score.hpp"

#include "ECS/utilities/Zipper/Zipper.hpp"
#include "Display.hpp"
#include "library_entrypoint.hpp"

Systems::Display::Display()
{
    InitWindow(1920, 1080, "Window");
    ToggleFullscreen();
    SetTargetFPS(60);

    __playerTexture = LoadTexture("./assets/leaf-blower-player.png");
    __leafTexture = LoadTexture("./assets/leaf-blower-leaf.png");
}

Systems::Display::Display(libconfig::Setting &config)
{
    unsigned int width = config.lookup("width");
    unsigned int height = config.lookup("height");
    const std::string &title = config.lookup("title");
    bool fullscreen = config.lookup("fullscreen");

    InitWindow(width, height, title.c_str());
    if (fullscreen)
        ToggleFullscreen();

    SetTargetFPS(60);

    __playerTexture = LoadTexture("./assets/leaf-blower-player.png");
    __leafTexture = LoadTexture("./assets/leaf-blower-leaf.png");
}

void Systems::Display::init(Engine::GameEngine &engine)
{
    engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position");
    engine.registerComponent<Components::Type>("./plugins/bin/components/", "Type");
    engine.registerComponent<Components::Rotation>("./plugins/bin/components/", "Rotation");
    engine.registerComponent<Components::Score>("./plugins/bin/components/", "Score");
}

void Systems::Display::run(Engine::GameEngine &engine)
{
    auto &componentManager = engine.getRegistry().componentManager();
    auto &posComponentArr = componentManager.getComponents<Components::Position>();
    auto &typeComponentArr = componentManager.getComponents<Components::Type>();
    auto &rotComponentArr = componentManager.getComponents<Components::Rotation>();
    auto &scoreComponentArr = componentManager.getComponents<Components::Score>();

    if (WindowShouldClose()) {
        CloseWindow();
        exit(0);
    }

    ClearBackground(LIME);
    BeginDrawing();

    // Ally is the player, enemies are the leaves
    for (auto &&[pos, type, rot] : Zipper(posComponentArr, typeComponentArr, rotComponentArr)) {
        if (type.id == Components::TypeID::ENEMY) {
            Vector2 origin = {21, 21};

            DrawTexturePro(__leafTexture,
                {0, 0, 42, 42},
                {(float)pos.x, (float)pos.y, 42, 42},
                origin, rot.z, RAYWHITE);
        }
    }
    for (auto &&[pos, type, rot, score] : Zipper(posComponentArr, typeComponentArr, rotComponentArr, scoreComponentArr)) {
        if (type.id == Components::TypeID::ALLY) {
            Vector2 origin = {80, 80};

            DrawTexturePro(__playerTexture,
                {0, 0, 160, 160},
                {(float)pos.x, (float)pos.y, 160, 160},
                origin, rot.z, RAYWHITE);

            std::string scoreStr = "Score: " + std::to_string(score.value);
            DrawText(scoreStr.c_str(), 10, 10, 20, BLACK);
        }
    }

    EndDrawing();
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryPoint()
{
    return new Systems::Display();
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryConfig(libconfig::Setting &config)
{
    return new Systems::Display(config);
}
