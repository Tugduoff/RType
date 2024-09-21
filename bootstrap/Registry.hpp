/*
** EPITECH PROJECT, 2024
** BsRtype
** File description:
** Registry.hpp file
*/

#ifndef REGISTRY_HPP
    #define REGISTRY_HPP

    #include "SparseArray.hpp"
    #include "Entity.hpp"
    #include "EntityManager.hpp"
    #include <any>
    #include <unordered_map>
    #include <typeindex>
    #include <functional>
    #include <unordered_set>

class Registry {
    public:

        // Component management

        template <class Component>
        SparseArray<Component> &register_component();

        template <class Component>
        SparseArray<Component> &get_components();

        template <class Component>
        SparseArray<Component> const &get_components() const;

        // Entity management

        Entity spawn_entity() { return __entityManager.spawn_entity(); };
        Entity entity_from_index(std::size_t idx) { return __entityManager.entity_from_index(idx); };
        void kill_entity(Entity const &e) { __entityManager.kill_entity(e); };

        template <typename Component>
        typename SparseArray<Component>::reference_type add_component(Entity const &to, Component &&c);

        template <typename Component, typename... Params>
        typename SparseArray<Component>::reference_type emplace_component(Entity const &to, Params &&...p);

        template <typename Component>
        void remove_component(Entity const &from);

        // Systems management

        template <class... Components, typename Function>
        void add_system(Function &&f);

        template <class... Components, typename Function>
        void add_system(Function const &&f);

        void run_systems();

    private:

        std::unordered_map<std::type_index, std::any> __components;
        std::vector<std::function<void(Registry &)>> __systems;
        EntityManager __entityManager;

};

// Implementation of the methods

// Systems

void Registry::run_systems() {
    int i = 0;

    for (auto &system : __systems) {
        system(*this);
        i++;
    }
}

template <class... Components, typename Function>
void Registry::add_system(Function &&f) {
    __systems.emplace_back([f = std::forward<Function>(f)](Registry &reg) {
        f(reg);
    });
}

template <class... Components, typename Function>
void Registry::add_system(Function const &&f) {
    __systems.emplace_back([f = std::forward<Function>(f)](Registry &reg) {
        f(reg);
    });
}

// Entity management

template <typename Component>
typename SparseArray<Component>::reference_type Registry::add_component(Entity const &to, Component &&c) {
    std::type_index typeIndex = std::type_index(typeid(Component));

    try {
        return std::any_cast<SparseArray<Component> &>(__components.at(typeIndex)).insert_at(to, std::move(c));
    } catch (const std::out_of_range &) {
        throw std::runtime_error("Component type not registered");
    }
}

template <typename Component, typename... Params>
typename SparseArray<Component>::reference_type Registry::emplace_component(Entity const &to, Params &&...p) {
    std::type_index typeIndex = std::type_index(typeid(Component));

    try {
        return std::any_cast<SparseArray<Component> &>(__components.at(typeIndex)).emplace_at(to, std::forward<Params>(p)...);
    } catch (const std::out_of_range &) {
        throw std::runtime_error("Component type not registered");
    }
}

template <typename Component>
void Registry::remove_component(Entity const &from) {
    std::type_index typeIndex = std::type_index(typeid(Component));

    try {
        std::any_cast<SparseArray<Component> &>(__components.at(typeIndex)).erase(from);
    } catch (const std::out_of_range &) {
        throw std::runtime_error("Component type not registered");
    }
}

// Component management

template <class Component>
SparseArray<Component> &Registry::register_component() {
    std::type_index typeIndex = std::type_index(typeid(Component));

    if (__components.find(typeIndex) != __components.end())
        throw std::runtime_error("Component already registered");
    __components[typeIndex] = SparseArray<Component>();
    return std::any_cast<SparseArray<Component> &>(__components[typeIndex]);
}

template <class Component>
SparseArray<Component> &Registry::get_components() {
    std::type_index typeIndex = std::type_index(typeid(Component));

    try {
        return std::any_cast<SparseArray<Component> &>(__components.at(typeIndex));
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Component type not registered");
    }
}

template <class Component>
SparseArray<Component> const &Registry::get_components() const {
    std::type_index typeIndex = std::type_index(typeid(Component));

    try {
        return std::any_cast<SparseArray<Component> const &>(__components.at(typeIndex));
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Component type not registered");
    }
}

#endif // REGISTRY_HPP
