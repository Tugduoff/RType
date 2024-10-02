/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ComponentRegisterer.hpp file
*/

#ifndef COMPONENT_REGISTERER_HPP
    #define COMPONENT_REGISTERER_HPP

    #include "ECS/component/ComponentManager.hpp"
    #include <functional>
    #include <typeindex>

namespace ECS {
    class ComponentManager;

    /**
     * @brief Wrapper for a `ComponentManager`'s `registerComponent` member function
     *
     * This class allows the GameEngine to allow the system plugins to register
     * their components in the ComponentManager of the Registry by passing
     * template parameters to the member function `registerComponent`
     */
    class ComponentRegisterer {
        public:
            using callBackType = std::function<
                bool(const std::string &plugin, std::type_index typeIndex)
            >;

            /**
             * @brief Constructor for a ComponentRegisterer
             *
             * @param manager The ComponentManager on which to call `registerComponent`
             * @param preHook Optional function to run before `registerComponent`
             * @param postHook Optional function to run after `registerComponent`
             */
            ComponentRegisterer(
                ComponentManager &manager,
                callBackType preHook = defaultHook,
                callBackType postHook = defaultHook
            ) : _linkedManager(manager), _preHook(preHook), _postHook(postHook) {}


            template<class Component>
            auto registerComponent(const std::string &plugin) const
            {
                return _preHook(plugin, std::type_index(typeid(Component)))
                    && _linkedManager.registerComponent<Component>()
                    && _postHook(plugin, std::type_index(typeid(Component)));
            }

            // Function that does nothing in the registerComponent member function
            constexpr static const auto defaultHook = [](auto, auto){
                return true;
            };

        protected:

            ComponentManager &_linkedManager;

            callBackType _preHook;
            callBackType _postHook;

    };
}

#endif /* COMPONENT REGISTERER */

