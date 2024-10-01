/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Main.cpp file
*/

#include "GameEngine/GameEngine.hpp"
#include "ECS/registry/Registry.hpp"
#include "ECS/utilities/SparseArray.hpp"
#include "plugins/components/IComponent.hpp"
#include "plugins/components/position/Position.hpp"
#include "plugins/components/velocity/Velocity.hpp"
#include <iostream>

int main() {
    Engine::GameEngine engine;
    std::string positionPluginPath = "./plugins/bin/components/libPosition.so";
    std::string configSystemPath = "./plugins/bin/systems/libConfig.so";

    try {
        ECS::Registry &reg = engine.getRegistry();
        ECS::Entity entity = reg.entityManager().spawnEntity();

        engine.loadSystems("./plugins/bin/systems/", "./plugins/bin/systems/configSystems.cfg");

        std::unique_ptr<Components::Position> position = engine.newComponent<Components::Position>(10, 20);
        std::unique_ptr<Components::Velocity> velocity = engine.newComponent<Components::Velocity>(2, 1);
        engine.addComponent<Components::Position>(entity, std::move(position));
        engine.addComponent<Components::Velocity>(entity, std::move(velocity));

        SparseArray<Components::Position> &positionComponents = reg.componentManager().getComponents<Components::Position>();
        std::cout << "Position components: " << std::endl;
        for (std::unique_ptr<Components::Position> &pos : positionComponents) {
            if (!pos)
                continue;
            std::cout << "Position: " << pos->x << ", " << pos->y << std::endl;
        }

        engine.runSystems();

        std::cout << "Position components: " << std::endl;
        for (std::unique_ptr<Components::Position> &pos : positionComponents) {
            if (!pos)
                continue;
            std::cout << "Position: " << pos->x << ", " << pos->y << std::endl;
        }
    } catch (std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
