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

    #define DEG_TO_RAD 0.017453292519943295769236907684886

namespace Systems {
    /**
     * @class DisplayManager
     * 
     * @brief System class from Systems that manages the display of the game MarioQuartz
     */
    class DisplayManager {
        public:

            DisplayManager();
            DisplayManager(libconfig::Setting &config);
            ~DisplayManager() = default;

            void init(Engine::GameEngine &engine);
            void run(Engine::GameEngine &engine);
            void freeAll(Engine::GameEngine &engine);

        private:

            /**
             * @brief Load the models from the modelsPath
             * 
             * @param modelsPath : the path to the models file
             * 
             * @note This function will load the models from the modelsPath and store them in the __models map
             */
            void loadModels(std::string &modelsPath);

            /**
             * @brief Draw the models in the scene
             * 
             * @param componentManager : the component manager
             * 
             * @note This function will draw the models in the scene
             */
            void rotateModels(ECS::ComponentManager &componentManager);

            /**
             * @brief Check for missing model components in the scene
             * 
             * @param componentManager : the component manager
             * 
             * @note If the model component is missing, but a modelId component exists for this entity, it will be added to the scene
             */
            void checkForMissingModelComponent(ECS::ComponentManager &componentManager);

            // First is modelId, second is modelPath
            std::unordered_map<std::string, std::string> __models;

    };
};

#endif // DISPLAY_MANAGER_HPP
