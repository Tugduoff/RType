/*
** EPITECH PROJECT, 2024
** RType
** File description:
** InputManager.hpp file
*/

#ifndef INPUT_MANAGER_HPP
    #define INPUT_MANAGER_HPP

    #include "systems/ASystem.hpp"
    #include "ECS/component/ComponentManager.hpp"
    #include "utils/Keys.hpp"
    #include <raylib.h>
    #include <libconfig.h++>
    #include <unordered_map>
    #include <memory>

    #define DEG_TO_RAD 0.017453292519943295769236907684886
    #define RAD_TO_DEG 57.295779513082320876798154814105

namespace Systems {
    /**
     * @class InputManager
     * 
     * @brief System class from Systems that manages the input of the game MarioQuartz
     */
    class InputManager {
        public:

            InputManager();
            InputManager(libconfig::Setting &config);
            ~InputManager() = default;

            void init(Engine::GameEngine &engine);
            void run(Engine::GameEngine &engine);

        private:

            /**
             * @brief Update the cameras in the scene
             * 
             * @param componentManager : the component manager
             */
            void updateCameras(ECS::ComponentManager &componentManager);

            /**
             * @brief Check for missing camera components in the scene
             * 
             * @param componentManager : the component manager
             * 
             * @note If the camera component is missing, but a controllable component exists for this entity, it will be added to the scene
             */
            void checkForMissingCameraComponent(ECS::ComponentManager &componentManager);

            /**
             * @brief Retrieve the inputs from the user
             * 
             * @param engine : the game engine
             */
            void retrieveInputs(Engine::GameEngine &engine);

            const std::unordered_map<int, enum Key> __intToKey = {
                {65, Key::Q}, {66, Key::B}, {67, Key::C}, {68, Key::D}, {69, Key::E}, {70, Key::F}, {71, Key::G}, {72, Key::H},
                {73, Key::I}, {74, Key::J}, {75, Key::K}, {76, Key::L}, {77, Key::M}, {78, Key::N}, {79, Key::O}, {80, Key::P},
                {81, Key::A}, {82, Key::R}, {83, Key::S}, {84, Key::T}, {85, Key::U}, {86, Key::V}, {87, Key::Z}, {88, Key::X},
                {89, Key::Y}, {90, Key::W},
                {38, Key::UP_KEY}, {40, Key::DOWN_KEY}, {37, Key::LEFT_KEY}, {39, Key::RIGHT_KEY},
                {32, Key::SPACE}, {13, Key::ENTER}, {27, Key::ESCAPE}, {46, Key::DELETE_KEY},
                {8, Key::BACKSPACE},
                {96, Key::NUM_0}, {97, Key::NUM_1}, {98, Key::NUM_2}, {99, Key::NUM_3},
                {100, Key::NUM_4}, {101, Key::NUM_5}, {102, Key::NUM_6}, {103, Key::NUM_7},
                {104, Key::NUM_8}, {105, Key::NUM_9},
                {-1, Key::LEFT_CLICK}, {-2, Key::RIGHT_CLICK}, {-3, Key::MIDDLE_CLICK}, {-4, Key::UNKNOWN}
            };
            /**
             * @brief Convert a key to an integer
             * 
             * @param key : the key to convert
             * 
             * @return The integer corresponding to the key
             */
            int keyToInt(Key &key);
    };
};

#endif // INPUT_MANAGER_HPP
