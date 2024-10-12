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
    #include <functional>
    #include <stdexcept>
    #include <unordered_map>
    #include <typeindex>
    #include <any>
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
             * @brief Register a component
             * 
             * @tparam Component : the component type
             * 
             * @return bool : true if the component got registered, false otherwise
             * 
             * @note This function will register a component in the component manager.
             * @note It's a major function in the ECS as it allows for components to be registered and then created.
             */
            template <class Component>
            bool registerComponent(std::function<Component *()> ctor)
            {
                std::type_index typeIndex = std::type_index(typeid(Component));

                if (__components.contains(typeIndex))
                    return (false);

                __components.emplace(typeIndex, std::make_any<SparseArray<Component>>(ctor));
                auto dtor = [
                    &sparseArr = std::any_cast<SparseArray<Component> &>(__components.at(typeIndex))
                ](const Entity &entity) {
                    sparseArr.erase(entity);
                };
                __compDestructors.emplace(typeIndex, std::move(dtor));
                return (true);
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
                return std::any_cast
                    <SparseArray<Component> &>
                    (getComponents(typeid(Component)));
            }

            /**
             * @brief Get components
             * 
             * @tparam type : the type index of the component
             * 
             * @return std::any &: the components sparseArray containing all instances as std::any
             * 
             * @note This function will retrieve all components from the same type given as template parameter.
             * @note Mostly used in systems to iterate over all components of a specific type.
             */
            std::any &getComponents(std::type_index type)
            {
                if (!__components.contains(type)) {
                    throw std::runtime_error("Component type not registered in component manager");
                }

                return __components.at(type);
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
                    throw std::runtime_error("Component type not registered in component manager");


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
                    throw std::runtime_error("Component type not registered in component manager");

                SparseArray<Component> &sparseArray = std::any_cast<SparseArray<Component>&>(
                    __components.at(typeIndex)
                );

                sparseArray.erase(from);
            }


            /**
             * @brief Remove a component from an entity
             * 
             * @param type : the component type index
             * @param from : the entity to remove the component from
             * 
             * @note This function will remove a component from an entity.
             */
            void removeComponent(std::type_index type, Entity const &from)
            {
                if (!__compDestructors.contains(type)) {
                    throw std::runtime_error("Component type not registered in component manager");
                }

                __compDestructors.at(type)(from);
            }


            /**
             * @brief Remove all components from an entity
             * 
             * @param from : the entity to remove all components from
             * 
             * @note This function will remove all components from an entity.
             */
            void removeAllFromEntity(Entity const &from)
            {
                for (auto const &[_, dtor] : __compDestructors) {
                    dtor(from);
                }
            }

        private:

            std::unordered_map<std::type_index, std::any> __components;
            std::unordered_map<
                std::type_index,
                std::function<void(const Entity &e)>
            > __compDestructors;

    };
};

#endif // COMPONENT_MANAGER_HPP
