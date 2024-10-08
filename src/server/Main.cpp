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
#include "../network/include/server/UDPServer.hpp"
#include <exception>
#include <iostream>

void displayComponents(ECS::Registry &reg)
{
    SparseArray<Components::Position> &positionComponents = reg.componentManager().getComponents<Components::Position>();
    SparseArray<Components::Velocity> &velocityComponents = reg.componentManager().getComponents<Components::Velocity>();

    std::cout << "Displaying components: " << std::endl;
    std::cout << "\nPosition components: \n" << std::endl;
    for (std::unique_ptr<Components::Position> &pos : positionComponents) {
        if (!pos)
            continue;
        std::cout << "Position: " << pos->x << ", " << pos->y << std::endl;
    }

    std::cout << "\nVelocity components: \n" << std::endl;
    for (std::unique_ptr<Components::Velocity> &vel : velocityComponents) {
        if (!vel)
            continue;
        std::cout << "Velocity: " << vel->x << ", " << vel->y << std::endl;
    }
}

int main() {
    Engine::GameEngine engine;
    ECS::Registry &reg = engine.getRegistry();
    ECS::Entity entity = reg.entityManager().spawnEntity();

    try {
        engine.loadSystems("./plugins/bin/systems/configSystems.cfg");

        std::unique_ptr<Components::Position> position = engine.newComponent<Components::Position>(10, 20, 1);
        std::unique_ptr<Components::Velocity> velocity = engine.newComponent<Components::Velocity>(2, 1);

        reg.componentManager().addComponent<Components::Position>(entity, std::move(position));
        reg.componentManager().addComponent<Components::Velocity>(entity, std::move(velocity));

        boost::asio::io_context io_context;
        UDPServer server(io_context, 8080, engine.getComponents());
        io_context.run();

        displayComponents(reg);
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
