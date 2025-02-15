/*
** EPITECH PROJECT, 2024
** RType
** File description:
** InputManager.cpp file
*/

#include "InputManager.hpp"
#include "components/controllable/Controllable.hpp"
#include "components/position3d/Position3d.hpp"
#include "components/rotation/Rotation.hpp"
#include "CameraComponent.hpp"
#include "ECS/utilities/Zipper/IndexedZipper.hpp"
#include <raymath.h>
#include <cmath>

float toDegrees(float radians) {
    return radians * RAD_TO_DEG;
}

float toRadians(float degrees) {
    return degrees * DEG_TO_RAD;
}

Systems::InputManager::InputManager()
{
}

Systems::InputManager::InputManager(libconfig::Setting &)
{
}

void Systems::InputManager::init(Engine::GameEngine &engine)
{
    engine.registerComponent<Components::Controllable>("./plugins/bin/components/", "Controllable");
    engine.registerComponent<Components::Position3d>("./plugins/bin/components/", "Position3d");
    engine.registerComponent<Components::Rotation>("./plugins/bin/components/", "Rotation");

    auto &componentManager = engine.getRegistry().componentManager();
    auto ctor = []() -> Components::CameraComponent * { return new Components::CameraComponent(); };
    componentManager.registerComponent<Components::CameraComponent>(ctor);
}

void Systems::InputManager::run(Engine::GameEngine &engine)
{
    ECS::ComponentManager &componentManager = engine.getRegistry().componentManager();
    checkForMissingCameraComponent(componentManager);
    updateCameras(componentManager);
    retrieveInputs(engine);
    auto &rotationComponents = componentManager.getComponents<Components::Rotation>();
    auto &cameraComponents = componentManager.getComponents<Components::CameraComponent>();

    for (auto &&[i, camera, rot] : IndexedZipper(cameraComponents, rotationComponents)) {
        // Update the rotation component accordingly to the camera
        rot.y = toDegrees(camera.mouseDelta.first);  // Yaw
        rot.floatY = toDegrees(camera.mouseDelta.first);  // Yaw
        rot.x = toDegrees(camera.mouseDelta.second); // Pitch
        rot.floatX = toDegrees(camera.mouseDelta.second); // Pitch

        // Integer modulo for rotation, ensuring values stay within [0, 360)
        rot.y = ((rot.y % 360) + 360) % 360;
        rot.x = ((rot.x % 360) + 360) % 360;

        // Floating-point modulo for rotation, ensuring values stay within [0, 360)
        rot.floatY = fmod(fmod(rot.floatY, 360.0f) + 360.0f, 360.0f);
        rot.floatX = fmod(fmod(rot.floatX, 360.0f) + 360.0f, 360.0f);

        if (rot.x < 0)
            rot.x += 360;
        if (rot.floatX < 0)
            rot.floatX += 360;
        if (rot.y < 0)
            rot.y += 360;
        if (rot.floatY < 0)
            rot.floatY += 360;

        // Roll (rot.z) can be set to zero or controlled separately, depending on the requirements
        rot.z = 0.0f;
        rot.floatZ = 0.0f;
        engine.updateComponent(i, rot.getId(), rot.serialize());
    }
}

void Systems::InputManager::retrieveInputs(Engine::GameEngine &engine)
{
    auto &controllableComponents = engine.getRegistry().componentManager().getComponents<Components::Controllable>();

    for (auto &&[i, ctrl] : IndexedZipper(controllableComponents)) {
        for (int j = 0; j < 4; j++) {
            // std::cerr << "Checking key: " << ctrl.keyBindings[(Action)i] << std::endl;
            if (IsKeyDown(keyToInt(ctrl.keyBindings[(Action)j])) && ctrl.inputs[j] == false) {
                std::cerr << "Key down: " << ctrl.keyBindings[(Action)j] << std::endl;
                ctrl.inputs[j] = true;
                engine.updateComponent(i, ctrl.getId(), ctrl.serialize());
            }
            if (IsKeyUp(keyToInt(ctrl.keyBindings[(Action)j])) && ctrl.inputs[j] == true) {
                std::cerr << "Key up: " << ctrl.keyBindings[(Action)j] << std::endl;
                ctrl.inputs[j] = false;
                engine.updateComponent(i, ctrl.getId(), ctrl.serialize());
            }
        }

        for (int j = 0; j < 10; j++) {
            if (IsKeyDown(keyToInt(ctrl.keyBindings[(Action)(j + 4)])) && ctrl.actions[j] == false) {
                ctrl.actions[j] = true;
                engine.updateComponent(i, ctrl.getId(), ctrl.serialize());
            }
            if (IsKeyUp(keyToInt(ctrl.keyBindings[(Action)(j + 4)]) && ctrl.actions[j] == true)) {
                ctrl.actions[j] = false;
                engine.updateComponent(i, ctrl.getId(), ctrl.serialize());
            }
        }
    }
}

void Systems::InputManager::updateCameras(ECS::ComponentManager &componentManager)
{
    auto &cameraComponents = componentManager.getComponents<Components::CameraComponent>();
    auto &positionComponents = componentManager.getComponents<Components::Position3d>();

    for (auto &&[i, camera, pos] : IndexedZipper(cameraComponents, positionComponents)) {
        const float sensitivity = 0.0001f;
        const float moveSpeed = 0.2f;
        const float pitchLimit = 0.1f * (PI / 2.0f);

        float &yaw = camera.mouseDelta.first;
        float &pitch = camera.mouseDelta.second;

        Vector2 mouseDelta = GetMouseDelta();

        yaw -= mouseDelta.x * sensitivity;
        pitch -= mouseDelta.y * sensitivity;

        pitch = Clamp(pitch, -pitchLimit, pitchLimit);

        Vector3 forward = { cosf(pitch) * sinf(yaw), sinf(pitch), cosf(pitch) * cosf(yaw) };
        camera.camera.target = Vector3Add(camera.camera.position, forward);

        camera.camera.position.x = pos.floatX;
        camera.camera.position.y = pos.floatY + 0.5;
        camera.camera.position.z = pos.floatZ;

        camera.camera.target = Vector3Add(camera.camera.position, forward);

        SetMousePosition(GetScreenWidth() / 2, GetScreenHeight() / 2);
    }
}

void Systems::InputManager::checkForMissingCameraComponent(ECS::ComponentManager &componentManager)
{
    auto &controllableComponents = componentManager.getComponents<Components::Controllable>();
    auto &cameraComponents = componentManager.getComponents<Components::CameraComponent>();

    for (auto &&[i, ctrl] : IndexedZipper(controllableComponents)) {
        try {
            auto &camera = cameraComponents[i];
            (void)camera;
        } catch (std::out_of_range &e) {
            std::cerr << "CameraComponent missing for entity: " << i << ". Creating it..." << std::endl;

            std::unique_ptr<Components::CameraComponent> cameraComp =
                std::make_unique<Components::CameraComponent>();

            componentManager
                .addComponent<Components::CameraComponent>((ECS::Entity)i, std::move(cameraComp));
        }
    }
}

int Systems::InputManager::keyToInt(Key &key) {
    for (auto &pair : __intToKey) {
        if (pair.second == key)
            return pair.first;
    }
    return -1;
}
