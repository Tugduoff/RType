/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ComponentUtils.hpp file
*/

#ifndef COMPONENT_UTILS_HPP
    #define COMPONENT_UTILS_HPP

    #include "GameEngine/GameEngine.hpp"
    #include "ECS/entity/Entity.hpp"
    #include <memory>

/**
 * @brief Attach a component to an entity and update it.
 * 
 * @tparam Component : the component type
 * @tparam ... Args : a pack of args
 * 
 * @param engine : the game engine
 * @param entity : the entity to add the component to
 * @param args : the arguments to pass to the component constructor
 */
template <typename Component, typename... Args>
void attachAndUpdateComponent(Engine::GameEngine &engine, ECS::Entity entity, Args&& ...args) {
    std::unique_ptr<Component> component = std::make_unique<Component>(std::forward<Args>(args)...);
    Components::IComponent *rawComponent = component.get();
    engine.getRegistry().componentManager().addComponent(entity, std::move(component));
    // engine.updateComponent(entity, rawComponent->getId(), rawComponent->serialize());
}

#endif // COMPONENT_UTILS_HPP
