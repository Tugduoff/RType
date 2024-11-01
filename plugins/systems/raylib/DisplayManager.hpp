/*
** EPITECH PROJECT, 2024
** RType
** File description:
** DisplayManager.hpp file
*/

#ifndef DISPLAY_MANAGER_HPP
    #define DISPLAY_MANAGER_HPP

    #include "systems/ASystem.hpp"
    #include "ECS/component/ComponentManager.hpp"
    #include <raylib.h>
    #include <libconfig.h++>
    #include <unordered_map>
    #include <memory>

namespace Systems {
    class DisplayManager {
        public:

            DisplayManager();
            DisplayManager(libconfig::Setting &config);
            ~DisplayManager() = default;

            void init(Engine::GameEngine &engine);
            void run(Engine::GameEngine &engine);
            void freeAll(Engine::GameEngine &engine);

        private:

            void loadModels(std::string &modelsPath);
            void rotateModels(ECS::ComponentManager &componentManager);
            void checkForMissingModelComponent(ECS::ComponentManager &componentManager);

            // First is modelId, second is modelPath
            std::unordered_map<std::string, std::string> __models;

    };
};

#endif // DISPLAY_MANAGER_HPP
