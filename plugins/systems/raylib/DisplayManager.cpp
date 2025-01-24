/*
** EPITECH PROJECT, 2024
** RType
** File description:
** DisplayManager.cpp file
*/

#include "DisplayManager.hpp"
#include "components/position3d/Position3d.hpp"
#include "components/position/Position.hpp"
#include "components/modelId/ModelId.hpp"
#include "components/scale/Scale.hpp"
#include "components/rotation/Rotation.hpp"
#include "components/text/Text.hpp"
#include "components/score/Score.hpp"
#include "components/marioType/MarioType.hpp"
#include "RaySprite.hpp"
#include "components/spriteId/SpriteID.hpp"
#include "ModelComponent.hpp"
#include "CameraComponent.hpp"
#include "ECS/utilities/Zipper/IndexedZipper.hpp"
#include "ECS/utilities/Zipper/Zipper.hpp"
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <cmath>

float toRadians2(float degrees) {
    return degrees * DEG_TO_RAD;
}

Systems::DisplayManager::DisplayManager()
{
    InitWindow(1920, 1080, "RType");

    // SetTargetFPS(60);
    DisableCursor();

    SetMousePosition(GetScreenWidth() / 2, GetScreenHeight() / 2);

    std::cerr << GetMousePosition().x << " " << GetMousePosition().y << std::endl;
    rlDisableBackfaceCulling();
    rlEnableDepthTest();

    __configFilePath = "./config/assets/textures.cfg";
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

    if (!config.lookupValue("texturesPath", __configFilePath))
        throw("Can't find configFilePath for all textures in Display config");

    InitWindow(width, height, title.c_str());

    if (fullscreen)
        ToggleFullscreen();

    loadModels(modelsPath);

    if (disableCursor)
        DisableCursor();

    // SetTargetFPS(fps);

    SetMousePosition(GetScreenWidth() / 2, GetScreenHeight() / 2);
    std::cerr << GetMousePosition().x << " " << GetMousePosition().y << std::endl;
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
    engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position");
    engine.registerComponent<Components::Rotation>("./plugins/bin/components/", "Rotation");
    engine.registerComponent<Components::Scale>("./plugins/bin/components/", "Scale");
    engine.registerComponent<Components::SpriteID>("./plugins/bin/components/", "SpriteID");
    engine.registerComponent<Components::Text>("./plugins/bin/components/", "Text");
    engine.registerComponent<Components::Score>("./plugins/bin/components/", "Score");
    engine.registerComponent<Components::MarioType>("./plugins/bin/components/", "MarioType");

    auto &componentManager = engine.getRegistry().componentManager();
    auto ctor = []() -> Components::ModelComponent * { return new Components::ModelComponent(); };
    componentManager.registerComponent<Components::ModelComponent>(ctor);
    auto ctor2 = []() -> Components::CameraComponent * { return new Components::CameraComponent(); };
    componentManager.registerComponent<Components::CameraComponent>(ctor2);
    auto ctor3 = []() -> Components::SpriteComponent * { return new Components::SpriteComponent(); };
    componentManager.registerComponent<Components::SpriteComponent>(ctor3);

    loadConfig(__configFilePath);
}

void Systems::DisplayManager::run(Engine::GameEngine &engine)
{
    ECS::ComponentManager &componentManager = engine.getRegistry().componentManager();

    auto &posComponents3d = componentManager.getComponents<Components::Position3d>();
    auto &posComponents = componentManager.getComponents<Components::Position>();
    auto &modelComponents = componentManager.getComponents<Components::ModelComponent>();
    auto &spriteComponents = componentManager.getComponents<Components::SpriteComponent>();
    auto &cameraComponents = componentManager.getComponents<Components::CameraComponent>();
    auto &spriteIdComponents = componentManager.getComponents<Components::SpriteID>();
    auto &scaleComponents = componentManager.getComponents<Components::Scale>();
    auto &textComponents = componentManager.getComponents<Components::Text>();
    auto &scoreComponents = componentManager.getComponents<Components::Score>();
    auto &marioTypeComponents = componentManager.getComponents<Components::MarioType>();

    checkForMissingModelComponent(componentManager);
    rotateModels(componentManager);

    BeginDrawing();
        BeginBlendMode(BLEND_ALPHA);
        ClearBackground(SKYBLUE);

        for (auto &&[camera] : Zipper(cameraComponents)) {
            // std::cerr << "Camera position: " << camera.camera.position.x << " " << camera.camera.position.y << " " << camera.camera.position.z << std::endl;
            BeginMode3D(camera.camera);

                for (auto &&[i, pos, model] : IndexedZipper(posComponents3d, modelComponents)) {
                    if (!model.isLoaded) {
                        std::cerr << "Model not loaded" << std::endl;
                        continue;
                    }

                    // std::cerr << "Model position: " << pos.floatX << " " << pos.floatY << " " << pos.floatZ << " and raw pos: " << pos.x << " " << pos.y << " " << pos.z << std::endl;
                    model.pos.x = static_cast<float>(pos.floatX);
                    model.pos.y = static_cast<float>(pos.floatY);
                    model.pos.z = static_cast<float>(pos.floatZ);

                    DrawModel(model.model, model.pos, 1.0f, WHITE);
                }

            EndMode3D();
        }

        // std::cerr << posComponents.size() << " " << spriteIdComponents.size() << " " << spriteComponents.size() << std::endl;
        for (auto &&[i, pos, spriteId] : IndexedZipper(posComponents, spriteIdComponents)) {
            if (spriteId.id.empty()) {
                continue;
            }

            Vector2 texturePos = {static_cast<float>(pos.x), static_cast<float>(pos.y)};
            Texture2D texture = GetTexture(spriteId.id.c_str());

            try {
                auto &sprite = spriteComponents[i];
                sprite->position = texturePos;
                sprite->update();

                // std::cerr << "Drawing entity: " << i << " at position: " << pos.x << ", " << pos.y << std::endl;

                try {
                    auto &scale = scaleComponents[i];

                    float scaleRatio = (scale->width != 0 ? scale->width : 100.0f) / 100.0f;

                    DrawTextureEx(texture, texturePos, 0, scaleRatio, WHITE);
                } catch (std::exception &e) {
                    // no scale component
                    DrawTextureRec(texture, sprite->rect, texturePos, WHITE);
                }

                // std::cerr << "Drawing entity: " << i << " at position: " << pos.x << ", " << pos.y << std::endl;
            } catch (std::exception &) {
                std::cerr << "Error: Sprite component not found for entity: " << i << " spriteID: " << spriteId.id << std::endl;
                std::unique_ptr<Components::SpriteComponent> sprite = std::make_unique<Components::SpriteComponent>();
                for (auto &texture : __textures[spriteId.id]) {
                    sprite->addTexture(texture);
                }

                try {
                    auto &scale = scaleComponents[i];

                    float scaleRatio = 100.0f / scale->width;

                    DrawTextureEx(texture, texturePos, 0, scaleRatio, WHITE);
                } catch (std::exception &e) {
                    // no scale component
                    DrawTextureRec(texture, sprite->rect, texturePos, WHITE);
                }
                componentManager.addComponent<Components::SpriteComponent>((ECS::Entity)i, std::move(sprite));
            }
        }

        // get the score.
        int scoreVal = 0;

        for (auto &&[i, type, score] : IndexedZipper(marioTypeComponents, scoreComponents)) {
            if (type.id == Components::MarioTypeID::PLAYER) {
                std::cerr << i << " " << score.value << std::endl;
                scoreVal = score.value;
            }
        }

        std::cerr << "Score: " << scoreVal << std::endl;

        for (auto &&[i, pos, text] : IndexedZipper(posComponents, textComponents)) {
            std::string textToDraw = text.value;

            if (text.id == Components::TextID::SCORE) {
                // find where the $ is and replace it with score.
                size_t pos = textToDraw.find("$");
                textToDraw.replace(pos, 1, std::to_string(scoreVal));
            }

            DrawText(textToDraw.c_str(), pos.x, pos.y, 20, WHITE);
        }

        EndBlendMode();
    EndDrawing();
}

Texture2D Systems::DisplayManager::GetTexture(const std::string &filepath)
{
    return __textures[filepath].at(0).getTexture();
}

void Systems::DisplayManager::loadConfig(const std::string &filepath)
{
    libconfig::Config cfg;

    try {
        cfg.readFile(filepath);
        std::cout << "\nConfig file loaded: " << filepath << std::endl;
        libconfig::Setting &root = cfg.getRoot();
        libconfig::Setting &textures = root["textures"];
        for (int i = 0; i < textures.getLength(); i++) {
            std::string type;
            std::string texturePath;
            std::string textureName;
            std::string actionName;
            unsigned updateRate = 0;
            unsigned rectX = 0;
            unsigned rectY = 0;

            textures[i].lookupValue("type", type);
            textures[i].lookupValue("name", textureName);
            if (type == "static") {
                textures[i].lookupValue("path", texturePath);

                std::vector<RayTexture> textures;

                Texture2D tempTexture;
                tempTexture = LoadTexture(texturePath.c_str());

                RayTexture texture(0, tempTexture.width, tempTexture.height, texturePath, "IDLE", false, false);
                textures.push_back(texture);
                __textures.insert({textureName, textures});
                continue;
            }
            if (type == "animated") {
                bool repeat = false;
                bool resetBeforeEnd = false;

                textures[i].lookupValue("path", texturePath);
                textures[i].lookupValue("updateRate", updateRate);
                textures[i].lookupValue("repeat", repeat);
                textures[i].lookupValue("resetBeforeEnd", resetBeforeEnd);

                libconfig::Setting &rectSetting = textures[i]["textureRect"];
                rectSetting.lookupValue("x", rectX);
                rectSetting.lookupValue("y", rectY);

                std::vector<RayTexture> textures;
                RayTexture texture(updateRate, rectX, rectY, texturePath, "IDLE", repeat, resetBeforeEnd);
                textures.push_back(texture);
                __textures.insert({textureName, textures});
                continue;
            }
            if (type == "actions") {
                libconfig::Setting &actions = textures[i]["actions"];
                std::vector<RayTexture> textures;

                for (int j = 0; j < actions.getLength(); j++) {
                    libconfig::Setting &action = actions[j];
                    action.lookupValue("type", type);

                    if (type == "static") {
                        action.lookupValue("path", texturePath);
                        action.lookupValue("action", actionName);

                        Texture2D tempTexture;
                        tempTexture = LoadTexture(texturePath.c_str());

                        RayTexture texture(0, tempTexture.width, tempTexture.height, texturePath, actionName, false, false);
                        textures.push_back(texture);
                        continue;
                    } else if (type == "animated") {
                        bool repeat = false;
                        bool resetBeforeEnd = false;

                        action.lookupValue("path", texturePath);
                        action.lookupValue("action", actionName);
                        action.lookupValue("updateRate", updateRate);
                        action.lookupValue("repeat", repeat);
                        action.lookupValue("resetBeforeEnd", resetBeforeEnd);

                        libconfig::Setting &rectSetting = action["textureRect"];
                        rectSetting.lookupValue("x", rectX);
                        rectSetting.lookupValue("y", rectY);
                        RayTexture texture(updateRate, rectX, rectY, texturePath, actionName, repeat, resetBeforeEnd);
                        textures.push_back(texture);
                        std::cerr << "Successfully loaded texture: " << textureName << " for action: " << actionName << std::endl;
                        continue;
                    }
                }
                __textures.insert({textureName, textures});
            }
        }
    } catch (libconfig::ParseException &e) {
        std::cerr << "Error while parsing file: "
            << e.getFile() << " in line: "
            << e.getLine() << " : "
            << e.getError() << std::endl;
    } catch (libconfig::FileIOException &e) {
        std::cerr << "Error while reading file: "
            << e.what() << std::endl;
        std::cerr << "File path was: " << filepath << std::endl;
    }
}

void Systems::DisplayManager::rotateModels(ECS::ComponentManager &componentManager)
{
    auto &rotationComponents = componentManager.getComponents<Components::Rotation>();
    auto &modelComponents = componentManager.getComponents<Components::ModelComponent>();

    for (auto &&[i, rot, model] : IndexedZipper(rotationComponents, modelComponents)) {
        // Convert rotation angles from degrees to radians
        float x = toRadians2(rot.floatX); // Pitch
        float y = toRadians2(rot.floatY); // Yaw
        float z = toRadians2(rot.floatZ); // Roll

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
