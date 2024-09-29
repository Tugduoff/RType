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
    class GameEngine {
        public:

            GameEngine() = default;
            ~GameEngine() {
                std::cout << "GameEngine destructor called" << std::endl;
                std::cout << "Component loaders cleared" << std::endl;
            };

            template <class Component>
            void registerComponent(const std::string &componentPath)
            {
                std::type_index typeIndex = std::type_index(typeid(Component));

                if (__componentLoaders.contains(typeIndex)) {
                    std::cerr << "Component already registered" << std::endl;
                    return;
                }

                __componentLoaders.emplace(typeIndex, DLLoader(componentPath));
                __registry.componentManager().registerComponent<Component>();
                std::cout << "Component loaded: " << componentPath << std::endl;
            }

            void loadSystems(const std::string &systemsFolderPath, const std::string &systemsConfigFile);

            template <typename Component, typename... Args>
            std::unique_ptr<Component> newComponent(Args&& ...args) {
                std::type_index typeIndex = std::type_index(typeid(Component));

                if (!__componentLoaders.contains(typeIndex))
                    throw std::runtime_error("Component type not registered");

                DLLoader &loader = __componentLoaders.at(typeIndex);
                auto componentInstance = loader.getInstance<Component>("entryPoint", std::forward<Args>(args)...);

                if (!componentInstance) {
                    throw std::runtime_error("Failed to load component from shared object");
                }
                return componentInstance;
            }

            template <typename Component>
            void addComponent(ECS::Entity &entity, std::unique_ptr<Component> component) {
                std::type_index typeIndex = std::type_index(typeid(Component));

                if (!__componentLoaders.contains(typeIndex))
                    throw std::runtime_error("Component type not registered");
                
                __registry.componentManager().addComponent<Component>(entity, std::move(component));
                std::cout << "Component added to entity: " << entity << std::endl;
            }

            ECS::Registry &getRegistry() { return __registry; }

        private:

            std::unordered_map<std::type_index, DLLoader> __componentLoaders;
            std::vector<DLLoader> __systemLoaders;
            ECS::Registry __registry;

    };
};

#endif // GAME_ENGINE_HPP
