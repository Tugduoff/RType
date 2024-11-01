/*
** EPITECH PROJECT, 2024
** RType
** File description:
** DisplayManager.cpp file
*/

#include "DisplayManager.hpp"
#include "components/position3d/Position3d.hpp"
#include "components/modelId/ModelId.hpp"
#include "components/scale/Scale.hpp"
#include "components/rotation/Rotation.hpp"
#include "ModelComponent.hpp"
#include "CameraComponent.hpp"
#include "ECS/utilities/Zipper/IndexedZipper.hpp"
#include "ECS/utilities/Zipper/Zipper.hpp"
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <cmath>

constexpr float DEG_TO_RAD = PI / 180.0f;

float toRadians2(float degrees) {
    return degrees * DEG_TO_RAD;
}

Systems::DisplayManager::DisplayManager()
{
    InitWindow(1920, 1080, "RType");

    // SetTargetFPS(60);
    DisableCursor();

    SetMousePosition(GetScreenWidth() / 2, GetScreenHeight() / 2);
    rlDisableBackfaceCulling();
    rlEnableDepthTest();
}

Systems::DisplayManager::DisplayManager(libconfig::Setting &config)
{
    unsigned int width = config.lookup("width");
    unsigned int height = config.lookup("height");
    bool fullscreen = config.lookup("fullscreen");
    std::string title = config.lookup("title");
    std::string modelsPath = config.lookup("modelsPath");
    unsigned int fps = config.lookup("fps");
    bool disableCursor = config.lookup("disableCursor");

    InitWindow(width, height, title.c_str());

    if (fullscreen)
        ToggleFullscreen();

    loadModels(modelsPath);

    if (disableCursor)
        DisableCursor();

    // SetTargetFPS(fps);

    SetMousePosition(GetScreenWidth() / 2, GetScreenHeight() / 2);
    rlDisableBackfaceCulling();
}

void Systems::DisplayManager::loadModels(std::string &modelsPath)
{
    libconfig::Config cfg;

    try {
        cfg.readFile(modelsPath);
        libconfig::Setting &root = cfg.getRoot();

        libconfig::Setting &models = root["models"];
        for (int i = 0; i < models.getLength(); ++i) {
            std::string modelName = models[i].lookup("name");
            std::string modelPath = models[i].lookup("path");

            __models.insert({
                modelName,
                modelPath
            });
        }
        std::cerr << "Loaded config file: " << modelsPath << " successfully!" << std::endl;
        if (__models.empty()) {
            std::cerr << "No models loaded" << std::endl;
            return;
        }
        std::cerr << "Models loaded: " << __models.size() << std::endl;
    } catch (libconfig::ParseException &e) {
        std::string err = "Error while parsing file: " + std::string(e.getFile()) + " in line: " + std::to_string(e.getLine()) + " : " + std::string(e.getError());
        throw std::runtime_error(err);
    } catch (libconfig::FileIOException &e) {
        std::string err = "Error while reading file: " + std::string(e.what()) + "\n" + "File path was: " + modelsPath;
        throw std::runtime_error(err);
    }
}

void Systems::DisplayManager::init(Engine::GameEngine &engine)
{
    engine.registerComponent<Components::ModelId>("./plugins/bin/components/", "ModelId");
    engine.registerComponent<Components::Position3d>("./plugins/bin/components/", "Position3d");
    engine.registerComponent<Components::Scale>("./plugins/bin/components/", "Scale");

    auto &componentManager = engine.getRegistry().componentManager();
    auto ctor = []() -> Components::ModelComponent * { return new Components::ModelComponent(); };
    componentManager.registerComponent<Components::ModelComponent>(ctor);
    auto ctor2 = []() -> Components::CameraComponent * { return new Components::CameraComponent(); };
    componentManager.registerComponent<Components::CameraComponent>(ctor2);
}

void Systems::DisplayManager::run(Engine::GameEngine &engine)
{
    ECS::ComponentManager &componentManager = engine.getRegistry().componentManager();

    auto &posComponents = componentManager.getComponents<Components::Position3d>();
    auto &modelComponents = componentManager.getComponents<Components::ModelComponent>();
    auto &cameraComponents = componentManager.getComponents<Components::CameraComponent>();

    checkForMissingModelComponent(componentManager);
    rotateModels(componentManager);

    BeginDrawing();
        BeginBlendMode(BLEND_ALPHA);
        ClearBackground(SKYBLUE);

        std::cerr << "Mode Drawing started" << std::endl;
        for (auto &&[camera] : Zipper(cameraComponents)) {
            // std::cerr << "Camera position: " << camera.camera.position.x << " " << camera.camera.position.y << " " << camera.camera.position.z << std::endl;
            BeginMode3D(camera.camera);

                std::cerr << "Mode 3D started" << std::endl;
                for (auto &&[i, pos, model] : IndexedZipper(posComponents, modelComponents)) {
                    if (!model.isLoaded) {
                        std::cerr << "Model not loaded" << std::endl;
                        continue;
                    }

                    // std::cerr << "Model position: " << pos.x << " " << pos.y << " " << pos.z << std::endl;
                    model.pos.x = static_cast<float>(pos.x);
                    model.pos.y = static_cast<float>(pos.y);
                    model.pos.z = static_cast<float>(pos.z);

                    std::cerr << "######### POSITION: " << std::setw(10) << std::setprecision(5) << model.pos.x << " " << std::setw(10) << std::setprecision(5) << model.pos.y << " " << std::setw(10) << std::setprecision(5) << model.pos.z << std::endl;
                    std::cerr << "######### POSITION: " << std::setw(10) << std::setprecision(5) << pos.x << " " << std::setw(10) << std::setprecision(5) << pos.y << " " << std::setw(10) << std::setprecision(5) << pos.z << std::endl;

                    DrawModel(model.model, model.pos, 1.0f, WHITE);
                }

            EndMode3D();
            for (auto &&[i, model] : IndexedZipper(modelComponents)) {
                if (model.pos.x == 0 && model.pos.y == 0 && model.pos.z == 0)
                    continue;
                DrawText(TextFormat("Model position: %f %f %f", model.pos.x, model.pos.y, model.pos.z), 10, 60, 20, WHITE);
            }
            DrawText(TextFormat("Camera position: %f %f %f", camera.camera.position.x, camera.camera.position.y, camera.camera.position.z), 10, 40, 20, WHITE);
        }
        EndBlendMode();
    EndDrawing();
}

void Systems::DisplayManager::rotateModels(ECS::ComponentManager &componentManager)
{
    auto &rotationComponents = componentManager.getComponents<Components::Rotation>();
    auto &modelComponents = componentManager.getComponents<Components::ModelComponent>();

    for (auto &&[i, rot, model] : IndexedZipper(rotationComponents, modelComponents)) {
        // Convert rotation angles from degrees to radians
        float x = toRadians2(rot.x); // Pitch
        float y = toRadians2(rot.y); // Yaw
        float z = toRadians2(rot.z); // Roll

        // Start with the identity matrix to clear previous transformations
        Matrix rotationMatrix = MatrixIdentity();

        // Apply rotation based on XYZ order (or any order that matches your needs)
        // rotationMatrix = MatrixMultiply(rotationMatrix, MatrixRotateX(x));
        rotationMatrix = MatrixMultiply(rotationMatrix, MatrixRotateY(y));
        rotationMatrix = MatrixMultiply(rotationMatrix, MatrixRotateZ(z));

        // Apply the new rotation matrix to the model
        model.model.transform = rotationMatrix;
    }
}

void Systems::DisplayManager::checkForMissingModelComponent(ECS::ComponentManager &componentManager)
{
    auto &modelIdComponents = componentManager.getComponents<Components::ModelId>();
    auto &modelComponents = componentManager.getComponents<Components::ModelComponent>();
    auto &scaleComponents = componentManager.getComponents<Components::Scale>();

    for (auto &&[i, modelId] : IndexedZipper(modelIdComponents)) {
        try {
            auto &model = modelComponents[i];
            (void)model;
        } catch (std::out_of_range &e) {
            std::cerr << "ModelComponent missing for entity: " << i << " with ModelId: " << modelId.id << ". Creating it..." << std::endl;

            std::unique_ptr<Components::ModelComponent> modelComp =
                std::make_unique<Components::ModelComponent>();
            modelComp->setModel(__models[modelId.id]);

            try {
                auto &scale = scaleComponents[i];
                float scaleValue = (float)scale->width / 100;
                Matrix scaleMatrix = MatrixScale(scaleValue, scaleValue, scaleValue);
                modelComp->model.transform = MatrixMultiply(modelComp->model.transform, scaleMatrix);
            } catch (std::out_of_range &e) {
                std::cerr << "ScaleComponent missing for entity: " << i << ". Using default scale..." << std::endl;
            }

            componentManager
                .addComponent<Components::ModelComponent>((ECS::Entity)i, std::move(modelComp));
        }
    }
}

void Systems::DisplayManager::freeAll(Engine::GameEngine &engine)
{
    auto &componentManager = engine.getRegistry().componentManager();
    auto &modelComponents = componentManager.getComponents<Components::ModelComponent>();

    for (auto &&[model] : Zipper(modelComponents)) {
        if (model.isLoaded)
            UnloadModel(model.model);
    }
    CloseWindow();
}
