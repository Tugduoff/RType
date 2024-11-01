/*
** EPITECH PROJECT, 2024
** RType
** File description:
** CameraComponent.hpp file
*/

#ifndef CAMERA_COMPONENT_HPP
    #define CAMERA_COMPONENT_HPP

    #include "systems/ASystem.hpp"
    #include <libconfig.h++>
    #include <unordered_map>
    #include <memory>
    #include <raylib.h>

namespace Components {
    class CameraComponent {
        public:

            CameraComponent() {
                camera.position = (Vector3){ 5.0f, 20.0f, 5.0f };
                camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
                camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
                camera.fovy = 45.0f;
                camera.projection = CAMERA_PERSPECTIVE;
            };
            ~CameraComponent() = default;

            void setPos(Vector3 pos) { camera.position = pos; }
            void setTarget(Vector3 target) { camera.target = target; }
            void setUp(Vector3 up) { camera.up = up; }
            void setFovy(float fovy) { camera.fovy = fovy; }
            void setProjection(CameraProjection projection) { camera.projection = projection; }

            Camera camera;
            std::pair<float, float> mouseDelta = { 0, 0 };

        private:

    };
};

#endif // CAMERA_COMPONENT_HPP
