/*
** EPITECH PROJECT, 2024
** RType
** File description:
** registry.hpp file
*/

#ifndef REGISTRY_HPP
    #define REGISTRY_HPP

    #include "sparseArray.hpp"
    #include "entity.hpp"
    #include <any>
    #include <unordered_map>
    #include <typeindex>
    #include <functional>
    #include <unordered_set>

class registry {
    public:

        // Component management

        template <class Component>
        sparseArray<Component> &register_component();

        template <class Component>
        sparseArray<Component> &get_components();

        template <class Component>
        sparseArray<Component> const &get_components() const;

        // Entity management

        entity spawn_entity();
        entity entity_from_index(std::size_t idx);
        void kill_entity(entity const &e);

        template <typename Component>
        typename sparseArray<Component>::reference_type add_component(entity const &to, Component &&c);

        template <typename Component, typename... Params>
        typename sparseArray<Component>::reference_type emplace_component(entity const &to, Params &&...p);

        template <typename Component>
        void remove_component(entity const &from);

        // Systems management

        template <class... Components, typename Function>
        void add_system(Function &&f);

        template <class... Components, typename Function>
        void add_system(Function const &&f);

        void run_systems();

    private:

        std::unordered_map<std::type_index, std::any> __components;
        std::vector<std::function<void(registry &)>> __systems;
        std::vector<entity> __entities;
        std::size_t __next_entity_id = 0;

};

// Implementation of the methods

// Systems

void registry::run_systems() {
    int i = 0;

    for (auto &system : __systems) {
        system(*this);
        i++;
    }
}

template <class... Components, typename Function>
void registry::add_system(Function &&f) {
    __systems.emplace_back([f = std::forward<Function>(f)](registry &reg) {
        f(reg);
    });
}

template <class... Components, typename Function>
void registry::add_system(Function const &&f) {
    __systems.emplace_back([f = std::forward<Function>(f)](registry &reg) {
        f(reg);
    });
}

// Entity management

entity registry::spawn_entity() {
    __entities.push_back(entity(__next_entity_id++));
    return __entities.back();
}

entity registry::entity_from_index(std::size_t idx) {
    if (idx >= __next_entity_id)
        throw std::out_of_range("Index out of bounds");
    return __entities[idx];
}

void registry::kill_entity(entity const &e) {
    __entities.erase(std::remove(__entities.begin(), __entities.end(), e), __entities.end());
}

// Component management

template <typename Component>
typename sparseArray<Component>::reference_type registry::add_component(entity const &to, Component &&c) {
    std::type_index typeIndex = std::type_index(typeid(Component));

    try {
        return std::any_cast<sparseArray<Component> &>(__components.at(typeIndex)).insert_at(to, std::move(c));
    } catch (const std::out_of_range &) {
        throw std::runtime_error("Component type not registered");
    }
}

template <typename Component, typename... Params>
typename sparseArray<Component>::reference_type registry::emplace_component(entity const &to, Params &&...p) {
    std::type_index typeIndex = std::type_index(typeid(Component));

    try {
        return std::any_cast<sparseArray<Component> &>(__components.at(typeIndex)).emplace_at(to, std::forward<Params>(p)...);
    } catch (const std::out_of_range &) {
        throw std::runtime_error("Component type not registered");
    }
}

template <typename Component>
void registry::remove_component(entity const &from) {
    std::type_index typeIndex = std::type_index(typeid(Component));

    try {
        std::any_cast<sparseArray<Component> &>(__components.at(typeIndex)).erase(from);
    } catch (const std::out_of_range &) {
        throw std::runtime_error("Component type not registered");
    }
}

template <class Component>
sparseArray<Component> &registry::register_component() {
    std::type_index typeIndex = std::type_index(typeid(Component));

    if (__components.find(typeIndex) != __components.end())
        throw std::runtime_error("Component already registered");
    __components[typeIndex] = sparseArray<Component>();
    return std::any_cast<sparseArray<Component> &>(__components[typeIndex]);
}

template <class Component>
sparseArray<Component> &registry::get_components() {
    std::type_index typeIndex = std::type_index(typeid(Component));

    try {
        return std::any_cast<sparseArray<Component> &>(__components.at(typeIndex));
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Component type not registered");
    }
}

template <class Component>
sparseArray<Component> const &registry::get_components() const {
    std::type_index typeIndex = std::type_index(typeid(Component));

    try {
        return std::any_cast<sparseArray<Component> const &>(__components.at(typeIndex));
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Component type not registered");
    }
}

#endif // REGISTRY_HPP
