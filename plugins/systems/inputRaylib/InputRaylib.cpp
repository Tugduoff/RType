/*
** EPITECH PROJECT, 2024
** RType
** File description:
** InputRaylibSystem.cpp file
*/

#include "InputRaylib.hpp"
#include "components/acceleration/Acceleration.hpp"
#include "components/controllable/Controllable.hpp"
#include "components/velocity3d/Velocity3d.hpp"
#include "components/rotation/Rotation.hpp"
#include "ECS/utilities/Zipper/IndexedZipper.hpp"
#include "library_entrypoint.hpp"
#include <cmath>

Systems::InputRaylibSystem::InputRaylibSystem(libconfig::Setting &)
{
}

inline float toRadians(float degrees) {
    return degrees * DEG_TO_RAD;
}

Vector3 calculateBackwardDirection(const Components::Rotation &rot) {
    float yaw = toRadians(-rot.y);

    Vector3 backwardDir;
    backwardDir.x = sin(yaw);
    backwardDir.y = 0.0f;
    backwardDir.z = -cos(yaw);

    return backwardDir;
}

Vector3 calculateForwardDirection(const Components::Rotation &rot) {
    Vector3 backwardDir = calculateBackwardDirection(rot);
    return Vector3{-backwardDir.x, -backwardDir.y, -backwardDir.z};
}

Vector3 calculateLeftDirection(const Components::Rotation &rot) {
    float yaw = toRadians(-rot.y);
    float pitch = toRadians(-rot.x);

    Vector3 leftDir;
    leftDir.x = cos(yaw) * cos(pitch);
    leftDir.y = sin(pitch);
    leftDir.z = sin(yaw) * cos(pitch);

    return leftDir;
}

Vector3 calculateRightDirection(const Components::Rotation &rot) {
    Vector3 leftDir = calculateLeftDirection(rot);
    return Vector3{-leftDir.x, -leftDir.y, -leftDir.z};
}

void Systems::InputRaylibSystem::run(Engine::GameEngine &engine) {
    auto &reg = engine.getRegistry();

    auto &controllableArr = reg.componentManager().getComponents<Components::Controllable>();
    auto &accelArr = reg.componentManager().getComponents<Components::Acceleration>();
    auto &velArr = reg.componentManager().getComponents<Components::Velocity3d>();
    auto &rotArr = reg.componentManager().getComponents<Components::Rotation>();

    for (auto &&[i, ctrl, accel, vel, rot] : IndexedZipper(controllableArr, accelArr, velArr, rotArr)) {
        bool inputForward = ctrl.inputs[(int)Action::FORWARD];
        bool inputBackward = ctrl.inputs[(int)Action::BACKWARD];
        bool inputRight = ctrl.inputs[(int)Action::RIGHT];
        bool inputLeft = ctrl.inputs[(int)Action::LEFT];

        Vector3 forwardDir = calculateForwardDirection(rot);
        Vector3 backwardDir = calculateBackwardDirection(rot);
        Vector3 rightDir = calculateRightDirection(rot);
        Vector3 leftDir = calculateLeftDirection(rot);

        vel.floatX = 0.0f;
        vel.floatY = 0.0f;
        vel.floatZ = 0.0f;

        if (inputForward) {
            vel.floatX += (float)accel.forward * forwardDir.x;
            vel.floatZ += (float)accel.forward * forwardDir.z;
        }
        if (inputBackward) {
            vel.floatX += (float)accel.backward * backwardDir.x;
            vel.floatZ += (float)accel.backward * backwardDir.z;
        }
        if (inputRight) {
            vel.floatX += (float)accel.right * rightDir.x;
            vel.floatZ += (float)accel.right * rightDir.z;
        }
        if (inputLeft) {
            vel.floatX += (float)accel.left * leftDir.x;
            vel.floatZ += (float)accel.left * leftDir.z;
        }

        for (int j = 0; j < 10; j++) {
            if (ctrl.actions[j]) {
                std::cerr << "Action " << j + 1 << " triggered" << std::endl;
            }
        }
    }
}

void Systems::InputRaylibSystem::init(Engine::GameEngine &engine)
{
    engine.registerComponent<Components::Acceleration>("./plugins/bin/components/", "Acceleration");
    engine.registerComponent<Components::Controllable>("./plugins/bin/components/", "Controllable");
    engine.registerComponent<Components::Velocity3d>("./plugins/bin/components/", "Velocity3d");
    engine.registerComponent<Components::Rotation>("./plugins/bin/components/", "Rotation");
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryPoint()
{
    return new Systems::InputRaylibSystem();
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryConfig(libconfig::Setting &config)
{
    return new Systems::InputRaylibSystem(config);
}
