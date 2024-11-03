/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ModelComponent.hpp file
*/

#ifndef MODEL_COMPONENT_HPP
    #define MODEL_COMPONENT_HPP

    #include "systems/ASystem.hpp"
    #include <raylib.h>
    #include <libconfig.h++>
    #include <unordered_map>
    #include <memory>

namespace Components {
    /**
     * @class ModelComponent
     * 
     * @brief Component class from Components that represents a model in the game
     */
    class ModelComponent {
        public:

            ModelComponent() = default;
            ~ModelComponent() = default;

            /**
             * @brief Set the Model object
             * 
             * @param model
             * 
             * @note This function sets the model object to the given model.
             */
            void setModel(std::string &modelPath);

            Model model;
            bool isLoaded = false;
            Vector3 pos = { 0, 0, 0 };

        private:

    };
};

#endif // MODEL_COMPONENT_HPP
