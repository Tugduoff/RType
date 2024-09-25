/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ComponentManager.hpp file
*/

#ifndef COMPONENT_MANAGER_HPP
    #define COMPONENT_MANAGER_HPP

    #include "ECS/entity/Entity.hpp"
    #include "ECS/utilities/SparseArray.hpp"
    #include "IComponent.hpp"
    #include "DLLoader.hpp"
    #include <unordered_map>
    #include <typeindex>
    #include <any>

namespace ECS {
    /**
     * @class ComponentManager
     * 
     * @brief Class that manages components inside the ECS
     */
    class ComponentManager {
        public:

            typedef struct component_s {
                std::any sparseArray;
                DLLoader *loader;
            } component_t;

            /**
             * @brief Load a component from a DLLoader
             * 
             * @tparam Args
             * @param loader
             * @param args
             * 
             * @return IComponent* : the loaded component
             */
            template <typename... Args>
            IComponent *loadComponent(DLLoader *loader, Args&&... args)
            {
                try {
                    return loader->getInstance<IComponent>("entryPoint", std::forward<Args>(args)...);
                } catch (DLLoader::DLLExceptions &e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                    return nullptr;
                }
            }

            /**
             * @brief Register a component
             * 
             * @tparam Component
             * 
             * @return SparseArray<Component>& : the registered component
             */
            template <class Component>
            SparseArray<Component> &registerComponent()
            {
                std::type_index typeIndex = std::type_index(typeid(Component));

                if (__components.find(typeIndex) != __components.end())
                    throw std::runtime_error("Component already registered");
                __components[typeIndex] = SparseArray<Component>();
                return std::any_cast<SparseArray<Component> &>(__components[typeIndex]);
            }

            /**
             * @brief Get components
             * 
             * @tparam Component
             * 
             * @return SparseArray<Component>& : the components
             */
            template <class Component>
            SparseArray<Component> &getComponents()
            {
                std::type_index typeIndex = std::type_index(typeid(Component));

                try {
                    return std::any_cast<SparseArray<Component> &>(__components.at(typeIndex));
                } catch (const std::out_of_range&) {
                    throw std::runtime_error("Component type not registered");
                }
            }

            /**
             * @brief Get components
             * 
             * @tparam Component
             * 
             * @return SparseArray<Component> const& : the components
             */
            template <class Component>
            SparseArray<Component> const &getComponents() const
            {
                std::type_index typeIndex = std::type_index(typeid(Component));

                try {
                    return std::any_cast<SparseArray<Component> const &>(__components.at(typeIndex));
                } catch (const std::out_of_range&) {
                    throw std::runtime_error("Component type not registered");
                }
            }

            /**
             * @brief Add a component to an entity
             * 
             * @tparam Component
             * @param to : the entity
             * @param c : the component
             * 
             * @return typename SparseArray<Component>::reference_type : the added component
             */
            template <typename Component>
            typename SparseArray<Component>::reference_type addComponent(Entity const &to, Component &&c)
            {
                std::type_index typeIndex = std::type_index(typeid(Component));

                try {
                    return std::any_cast<SparseArray<Component> &>(__components.at(typeIndex)).insert_at(to, std::move(c));
                } catch (const std::out_of_range &) {
                    throw std::runtime_error("Component type not registered");
                }
            }

            /**
             * @brief Remove a component from an entity
             * 
             * @tparam Component
             * @param from : the entity
             */
            template <typename Component>
            void removeComponent(Entity const &from)
            {
                std::type_index typeIndex = std::type_index(typeid(Component));

                try {
                    std::any_cast<SparseArray<Component> &>(__components.at(typeIndex)).erase(from);
                } catch (const std::out_of_range &) {
                    throw std::runtime_error("Component type not registered");
                }
            }

        private:

            std::unordered_map<std::type_index, component_t> __components;

    };
};

#endif // COMPONENT_MANAGER_HPP
