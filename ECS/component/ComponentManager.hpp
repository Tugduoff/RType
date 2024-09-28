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
    #include "DLLoader/DLLoader.hpp"
    #include <unordered_map>
    #include <typeindex>
    #include <any>
    #include <iostream>
    #include <utility>
    #include <memory>

namespace ECS {
    /**
     * @class ComponentManager
     * 
     * @brief Class that manages components inside the ECS
     */
    class ComponentManager {
        public:

            struct component {
                component(std::any sparseArray, DLLoader loader) : loader(std::move(loader)), sparseArray(std::move(sparseArray)) {};
                DLLoader loader;
                std::any sparseArray;
            };

            /**
             * @brief Load a new component instance
             * 
             * @tparam Args : the arguments to be given to the ctor of the component. Can be empty
             * @tparam Component : the component to load
             * @param args : the arguments to be given to the ctor of the component
             * 
             * @return IComponent* : the new instance of the component
             * 
             * @note This function is needed for loading an instance
             * @note of a specific component from type passed as template parameter.
             * @note It takes a parameter pack to pass to the constructor of the component.
             */
            template <class Component, typename... Args>
            IComponent *loadComponent(Args&&... args)
            {
                std::type_index typeIndex = std::type_index(typeid(Component));

                if (!__components.contains(typeIndex))
                    throw std::runtime_error("Component type not registered");
                
                DLLoader &loader = __components[typeIndex].loader;

                try {
                    return loader.getInstance<IComponent>("entryPoint", std::forward<Args>(args)...);
                } catch (DLLoader::DLLExceptions &e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                    return nullptr;
                }
            }

            /**
             * @brief Register a component
             * 
             * @tparam Component : the component to register
             * @param filepath : the filepath to the component library
             * 
             * @note This function must be called when loading a component library.
             * @note It will register the component type in the component manager and
             * @note allow for instances of this type to be created.
             */
            template <class Component>
            void registerComponent(std::string &libName)
            {
                std::type_index typeIndex = std::type_index(typeid(Component));

                if (__components.contains(typeIndex))
                    return;

                component comp = {
                    SparseArray<Component>(),
                    DLLoader(libName)
                };

                __components[typeIndex] = comp;
            }

            /**
             * @brief Get components
             * 
             * @tparam Component : the component type
             * 
             * @return SparseArray<Component>& : the components sparseArray containing all instances
             * 
             * @note This function will retrieve all components from the same type given as template parameter.
             * @note Mostly used in systems to iterate over all components of a specific type.
             */
            template <class Component>
            SparseArray<Component> &getComponents()
            {
                std::type_index typeIndex = std::type_index(typeid(Component));

                if (!__components.contains(typeIndex))
                    throw std::runtime_error("Component type not registered");

                return std::any_cast<SparseArray<Component> &>(__components.at(typeIndex).sparseArray);
            }

            /**
             * @brief Add a component to an entity
             * 
             * @tparam Component : the component type
             * @param to : the entity
             * @param c : the component
             * 
             * @return typename SparseArray<Component>::reference_type : the added component
             * 
             * @note This function will add a component to an entity.
             * @note It's a major function in the ECS as it allows for entities to have components.
             */
            template <typename Component>
            typename SparseArray<Component>::reference_type addComponent(Entity const &to, Component &&c)
            {
                std::type_index typeIndex = std::type_index(typeid(Component));

                if (!__components.contains(typeIndex))
                    throw std::runtime_error("Component type not registered");

                SparseArray<Component> &sparseArray = std::any_cast<SparseArray<Component>&>(__components.at(typeIndex).sparseArray);

                return sparseArray.insertAt(to, std::forward<Component>(c));            
            }

            /**
             * @brief Remove a component from an entity
             * 
             * @tparam Component : the component type
             * @param from : the entity to remove the component from
             * 
             * @note This function will remove a component from an entity.
             */
            template <typename Component>
            void removeComponent(Entity const &from)
            {
                std::type_index typeIndex = std::type_index(typeid(Component));

                if (!__components.contains(typeIndex))
                    throw std::runtime_error("Component type not registered");

                std::any_cast<SparseArray<Component>&>(__components.at(typeIndex).sparseArray).erase(from);
            }

        private:

            std::unordered_map<std::type_index, component> __components;

    };
};

#endif // COMPONENT_MANAGER_HPP
