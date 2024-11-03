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
    /**
     * @class CameraComponent
     * 
     * @brief Component class from Components that represents a camera in the game
     */
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

            /**
             * @brief Set the position of the camera
             * 
             * @param pos : the position to set
             */
            void setPos(Vector3 pos) { camera.position = pos; }

            /**
             * @brief Set the target of the camera
             * 
             * @param target : the target to set
             */
            void setTarget(Vector3 target) { camera.target = target; }

            /**
             * @brief Set the up vector of the camera
             * 
             * @param up : the up vector to set
             */
            void setUp(Vector3 up) { camera.up = up; }

            /**
             * @brief Set the fovy of the camera
             * 
             * @param fovy : the fovy to set
             */
            void setFovy(float fovy) { camera.fovy = fovy; }

            /**
             * @brief Set the projection of the camera
             * 
             * @param projection : the projection to set
             */
            void setProjection(CameraProjection projection) { camera.projection = projection; }

            Camera camera;
            std::pair<float, float> mouseDelta = { 0, 0 };

        private:

    };
};

#endif // CAMERA_COMPONENT_HPP
