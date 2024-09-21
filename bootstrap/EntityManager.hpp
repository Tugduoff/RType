/*
** EPITECH PROJECT, 2024
** RType
** File description:
** EntityManager.hpp file
*/

#ifndef ENTITY_MANAGER_HPP
    #define ENTITY_MANAGER_HPP

    #include "Entity.hpp"
    #include <algorithm>
    #include <stdexcept>
    #include <vector>

class EntityManager {
    public:

        Entity spawn_entity();
        Entity entity_from_index(size_t idx);
        void kill_entity(Entity const &e);

    private:

        std::vector<Entity> __entities;
        size_t __next_entity_id = 0;
};

// Implementation of the methods

Entity EntityManager::spawn_entity() {
    __entities.push_back(Entity(__next_entity_id++));
    return __entities.back();
}

Entity EntityManager::entity_from_index(size_t idx) {
    if (idx >= __next_entity_id)
        throw std::out_of_range("Index out of bounds");
    return __entities[idx];
}

void EntityManager::kill_entity(Entity const &e) {
    __entities.erase(std::remove(__entities.begin(), __entities.end(), e), __entities.end());
}

#endif // ENTITY_MANAGER_HPP
