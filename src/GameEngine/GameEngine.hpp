/*
** EPITECH PROJECT, 2024
** RType
** File description:
** GameEngine.hpp file
*/

#ifndef GAME_ENGINE_HPP
    #define GAME_ENGINE_HPP

    #include "ECS/registry/Registry.hpp"
    #include "DLLoader/DLLoader.hpp"
    #include <unordered_map>
    #include <typeindex>
    #include <iostream>

namespace Engine {
    /**
     * @class GameEngine
     * 
     * @brief Class that manages the game engine and stores the loaders of all plugins.
     */
    class GameEngine {
        public:

            /**
             * @brief Register a component
             * 
             * @tparam Component : the component type
             * 
             * @param componentPath : the path to the shared object containing the component
             * 
             * @return bool : true if the component got registered, false otherwise
             * 
             * @note This function will register a component in the component manager.
             * @note It will also store the loader in the component loaders map.
             * @note This function only needs to be called at the start of the program.
             * @note Possibly by the systems init functions.
             */
            template <class Component>
            bool registerComponent(const std::string &libPath, const std::string &libName)
            {
                std::type_index typeIndex = std::type_index(typeid(Component));

                if (__componentLoaders.contains(typeIndex))
                    return (false);

                __componentLoaders.emplace(typeIndex, DLLoader(libPath, libName));
                return __registry.componentManager().registerComponent<Component>();
            }

            /**
             * @brief Load systems from a configuration file
             * 
             * @param systemsFolderPath : the path to the folder containing the shared objects
             * @param systemsConfigFile : the path to the configuration file
             * 
             * @note This function will load systems from a configuration file.
             * @note Read the documentation about defining systems used in the game using the config file.
             * @note This function will load each systems and store them in the systemManager
             * @note and keep each loaders in the systems loaders vector.
             */
            void loadSystems(const std::string &systemsFolderPath, const std::string &systemsConfigFile);

            /**
             * @brief Run all systems
             * 
             * @note This function will run all systems in the system manager.
             * @note It must be called every tick of the game loop.
             */
            void runSystems();

            /**
             * @brief Initialize all systems
             * 
             * @note This function will run all systems init function.
             * @note It must be called at the start of the game.
             */
            void initSystems();

            /**
             * @brief Create a new component instance
             * 
             * @tparam Component : the component type
             * @tparam Args : the arguments to pass to the component constructor
             * 
             * @param args : the arguments to pass to the component constructor
             * 
             * @return std::unique_ptr<Component> : the component instance
             * 
             * @note This function will create a new component instance from a component tparam and stack of arguments.
             * @note It will use the component loaders map to load the component from the shared object.
             * @note And then return the component instance.
             * 
             * @throw std::runtime_error : "Component type not registered"
             * @throw std::runtime_error : "Failed to load component from shared object"
             */
            template <typename Component, typename... Args>
            std::unique_ptr<Component> newComponent(Args&& ...args) {
                std::type_index typeIndex = std::type_index(typeid(Component));

                if (!__componentLoaders.contains(typeIndex))
                    throw std::runtime_error("Component type not registered");

                DLLoader &loader = __componentLoaders.at(typeIndex);
                auto componentInstance = loader.getInstance<Component>("entryPoint", std::forward<Args>(args)...);

                if (!componentInstance)
                    throw std::runtime_error("Failed to load component from shared object");

                return componentInstance;
            }

            /**
             * @brief Get the registry
             * 
             * @return ECS::Registry& : the registry
             * 
             * @note This function will return the registry stored in the game engine.
             */
            ECS::Registry &getRegistry() { return __registry; }

        private:

            std::unordered_map<std::type_index, DLLoader> __componentLoaders;
            std::vector<DLLoader> __systemLoaders;
            ECS::Registry __registry;

    };
};

#endif // GAME_ENGINE_HPP
