/*
** EPITECH PROJECT, 2024
** RType
** File description:
** IEngine.hpp file
*/

#ifndef IENGINE_HPP
    #define IENGINE_HPP

    #include <string>
    #include <memory>

namespace Components {
    class IComponent;
}

namespace ECS {
    class Registry;
}

namespace Engine {
    class IEngine {
        public:
            virtual ~IEngine() = default;

            /**
             * @brief Load systems from a configuration file
             * 
             * @param systemsConfigFile : the path to the configuration file
             * 
             * @note This function will load systems from a configuration file.
             * @note Read the documentation about defining systems used in the game using the config file.
             * @note This function will load each systems and store them in the systemManager
             * @note and keep each loaders in the systems loaders vector.
             */
            virtual void loadSystems(const std::string &systemsConfigFile) = 0;

            /**
             * @brief Run all systems
             * 
             * @note This function will run all systems in the system manager.
             * @note It must be called every tick of the game loop.
             */
            virtual void runSystems() = 0;

            /**
             * @brief Initialize all systems
             * 
             * @note This function will run all systems init function.
             * @note It must be called at the start of the game.
             */
            virtual void initSystems() = 0;

            /**
             * @brief Get a component from its ID
             * 
             * @param componentId : the component ID
             * 
             * @return std::shared_ptr<Components::IComponent> : the component
             * 
             * @note This function will return a component from its ID.
             * @note It will return nullptr if the component is not found.
             */
            virtual std::shared_ptr<Components::IComponent> getComponentFromId(const std::string &componentId) = 0;

            /**
             * @brief Get the registry
             * 
             * @return ECS::Registry& : the registry
             * 
             * @note This function will return the registry stored in the game engine.
             */
            virtual ECS::Registry &getRegistry() = 0;
    };
};

#endif // IENGINE_HPP
