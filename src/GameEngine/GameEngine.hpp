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
    #include "plugins/components/IComponent.hpp"
    #include <memory>
    #include <string>
    #include <unordered_map>
    #include <typeindex>
    #include <iostream>

/**
 * @namespace Engine
 * 
 * @brief Namespace for the game Engine
 */
namespace Engine {
    /**
     * @class GameEngine
     * 
     * @brief Engine class that stores the loaders of all plugins.
     */
    class GameEngine {
        public:

            GameEngine(std::function<void(size_t, std::string, std::vector<uint8_t>)> updateComponent) : __updateComponent(updateComponent) {};

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

                if (!__componentLoaders.emplace(typeIndex, DLLoader(libPath, libName)).second)
                    return (false);

                DLLoader &loader = __componentLoaders.at(typeIndex);
                std::string componentID = loader.getSymbolValue<const char *>("componentName");

                std::cout << "Component ID: " << componentID << " registered!" << std::endl;

                auto compCtor = loader.getFunctionPointer<Component *>("buildDefault");

                __components.emplace(typeIndex, std::unique_ptr<Component>(compCtor()));
                __idStringToType.emplace(std::move(componentID), typeIndex);

                return __registry
                    .componentManager()
                    .registerComponent<Component>(compCtor);
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
            void loadSystems(const std::string &systemsConfigFile);

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
                auto componentInstance = loader.getUniqueInstance<Component>("buildWithParams", std::forward<Args>(args)...);

                if (!componentInstance)
                    throw std::runtime_error("Failed to load component from shared object");

                return componentInstance;
            }

            std::unique_ptr<Components::IComponent> &getComponentFromId(const std::string &componentId)
            {
                if (!__idStringToType.contains(componentId))
                    throw std::runtime_error("Component not found");

                return __components.at(__idStringToType.at(componentId));
            }

            std::unique_ptr<Components::IComponent> &getComponentFromId(const std::type_index &typeIndex)
            {
                if (!__components.contains(typeIndex))
                    throw std::runtime_error("Component not found");

                return __components.at(typeIndex);
            }

            /**
             * @brief Get the registry
             * 
             * @return ECS::Registry& : the registry
             * 
             * @note This function will return the registry stored in the game engine.
             */
            ECS::Registry &getRegistry() { return __registry; }

            /**
             * @brief Call the updateComponent call back
             * 
             * @param index : the index of the component
             * @param componentId : the id of the component
             * @param data : the data to update the component with
             * 
             * @note This function will call the updateComponent call back function.
             * @note It will be used by the systems to update components.
             * @note It will be set by the game engine constructor.
             */
            void updateComponent(size_t index, const std::string &componentId, std::vector<uint8_t> data)
            {
                __updateComponent(index, componentId, data);
            }

            std::unordered_map<std::string, std::unique_ptr<Components::IComponent>> &getComponents() { return __components; }

        private:

            std::unordered_map<std::type_index, DLLoader> __componentLoaders;
            std::vector<DLLoader> __systemLoaders;
            std::unordered_map<std::type_index, std::unique_ptr<Components::IComponent>> __components;
            std::unordered_map<std::string, std::type_index> __idStringToType;
            ECS::Registry __registry;
            std::function<void(size_t, std::string, std::vector<uint8_t>)> __updateComponent;

    };
};

#endif // GAME_ENGINE_HPP
