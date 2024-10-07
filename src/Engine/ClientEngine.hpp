/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ClientEngine.hpp file
*/

#ifndef CLIENT_ENGINE_HPP
    #define CLIENT_ENGINE_HPP

    #include "AEngine.hpp"
    #include "ECS/registry/Registry.hpp"
    #include "DLLoader/DLLoader.hpp"
    #include <unordered_map>
    #include <typeindex>
    #include <iostream>

namespace Engine {
    class ClientEngine : public AEngine {
        public:
            ClientEngine() = default;
            ~ClientEngine() = default;

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
                __components.emplace(componentID, std::make_shared<Component>());
                return __registry.componentManager().registerComponent<Component>();
            }

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
            void loadSystems(const std::string &systemsConfigFile) override;

            /**
             * @brief Run all systems
             * 
             * @note This function will run all systems in the system manager.
             * @note It must be called every tick of the game loop.
             */
            void runSystems() override;

            /**
             * @brief Initialize all systems
             * 
             * @note This function will run all systems init function.
             * @note It must be called at the start of the game.
             */
            void initSystems() override;

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
            std::unique_ptr<Component> newComponent(Args&& ...args)
            {
                std::type_index typeIndex = std::type_index(typeid(Component));

                if (!__componentLoaders.contains(typeIndex))
                    throw std::runtime_error("Component type not registered");

                DLLoader &loader = __componentLoaders.at(typeIndex);
                auto componentInstance = loader.getUniqueInstance<Component>("entryPoint", std::forward<Args>(args)...);

                if (!componentInstance)
                    throw std::runtime_error("Failed to load component from shared object");

                return componentInstance;
            }

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
            std::shared_ptr<Components::IComponent> getComponentFromId(const std::string &componentId) override;

            /**
             * @brief Get the registry
             * 
             * @return ECS::Registry& : the registry
             * 
             * @note This function will return the registry stored in the game engine.
             */
            ECS::Registry &getRegistry() { return __registry; };

        private:

            std::unordered_map<std::type_index, DLLoader> __componentLoaders;
            std::vector<DLLoader> __systemLoaders;
            std::unordered_map<std::string, std::shared_ptr<Components::IComponent>> __components;
            ECS::Registry __registry;

    };
};

#endif // CLIENT_ENGINE_HPP
