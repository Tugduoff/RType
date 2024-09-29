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

namespace Engine {
    class GameEngine {
        public:

            GameEngine() = default;
            ~GameEngine() = default;

            template <class Component>
            void registerComponent(const std::string &componentPath)
            {
                std::type_index typeIndex = std::type_index(typeid(Component));

                if (__componentLoaders.contains(typeIndex))
                    return;

                DLLoader loader(componentPath);

                __componentLoaders.emplace(typeIndex, loader);
            }

            void loadSystems(const std::string &systemsFolderPath, const std::string &systemsConfigFile);

            template <typename Component, typename... Args>
            std::unique_ptr<Component> newComponent(Args&& ...args) {
                std::type_index typeIndex = std::type_index(typeid(Component));

                if (!__componentLoaders.contains(typeIndex))
                    throw std::runtime_error("Component type not registered");

                DLLoader &loader = __componentLoaders.at(typeIndex);
                std::unique_ptr<Component> component = loader.getInstance<Component>("entryPoint", std::forward<Args>(args)...);

                return component;
            }

            ECS::Registry &getRegistry() { return __registry; }

        private:

            ECS::Registry __registry;
            std::unordered_map<std::type_index, DLLoader> __componentLoaders;

    };
};

#endif // GAME_ENGINE_HPP
