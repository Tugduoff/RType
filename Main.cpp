/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Main.cpp file
*/

#include "ECS/registry/Registry.hpp"
#include "ECS/utilities/SparseArray.hpp"
#include "plugins/components/IComponent.hpp"
#include "plugins/components/position/Position.hpp"

int main() {
    ECS::Registry reg;
    std::string positionPluginPath = "./plugins/bin/components/Position.so";

    try {
        reg.componentManager().registerComponent<Components::Position>(positionPluginPath);
        ECS::Entity entity = reg.entityManager().spawnEntity();
        std::unique_ptr<Components::Position> posComponent = reg.componentManager().loadComponent<Components::Position>(10, 20);
        reg.componentManager().addComponent<Components::Position>(entity, std::move(posComponent));

        SparseArray<Components::Position> &positionComponents = reg.componentManager().getComponents<Components::Position>();
        for (std::unique_ptr<Components::Position> &pos : positionComponents) {
            std::cout << "Position: " << pos->x << ", " << pos->y << std::endl;
        }
    } catch (std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
