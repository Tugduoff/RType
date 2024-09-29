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

                return std::any_cast<SparseArray<Component> &>(__components.at(typeIndex));
            }

            /**
             * @brief Add a component to an entity
             * 
             * @tparam Component : the component type
             * @param to : the entity
             * @param c : the component
             * 
             * @note This function will add a component to an entity.
             * @note It's a major function in the ECS as it allows for entities to have components.
             */
            template <class Component>
            void addComponent(Entity const &to, std::unique_ptr<Component> &&c)
            {
                std::type_index typeIndex = std::type_index(typeid(Component));

                if (!__components.contains(typeIndex))
                    throw std::runtime_error("Component type not registered");

                SparseArray<Component> &sparseArray = std::any_cast<SparseArray<Component>&>(
                    __components.at(typeIndex)
                );

                return sparseArray.insertAt(to, std::move(c));
            }

            /**
             * @brief Remove a component from an entity
             * 
             * @tparam Component : the component type
             * @param from : the entity to remove the component from
             * 
             * @note This function will remove a component from an entity.
             */
            template <class Component>
            void removeComponent(Entity const &from)
            {
                std::type_index typeIndex = std::type_index(typeid(Component));

                if (!__components.contains(typeIndex))
                    throw std::runtime_error("Component type not registered");

                std::any_cast<SparseArray<Component>&>(__components.at(typeIndex)).erase(from);
            }

        private:

            std::unordered_map<std::type_index, std::any> __components;

    };
};

#endif // COMPONENT_MANAGER_HPP
