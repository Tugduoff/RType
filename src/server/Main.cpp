/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Main.cpp file
*/


#include <exception>
#include <iostream>
#include <typeindex>
#include <vector>
#include "network/UDPServer.hpp"
#include "GameEngine/GameEngine.hpp"
#include "ECS/registry/Registry.hpp"
#include "ECS/utilities/SparseArray.hpp"
#include "plugins/components/IComponent.hpp"
#include "plugins/components/position/Position.hpp"
#include "plugins/components/velocity/Velocity.hpp"
#include "plugins/components/controllable/Controllable.hpp"

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

template<typename It>
void displayPolymorphic(Engine::GameEngine &engine, It begin, It end)
{
    int i = 0;

    for (auto it = begin; it != end; ++it) {
        std::type_index &idx = *it;

        std::cout << "For type index {" << idx.name() << "}:" << std::endl;

        auto &dummyComp = engine.getComponentFromId(idx);

        auto &sparseArray = dummyComp->any_cast(
            engine.getRegistry().componentManager().getComponents(idx)
        );

        for (auto const &comp : sparseArray) {
            std::cout << "    " << comp->getId() << ": {";
            i = 0;
            for (const auto &byte : comp->serialize()) {
                if (i != 0)
                    std::cout << ", ";
                std::cout << (int)byte;
                i++;
            }
            std::cout << "}" << std::endl;
        }
    }
}

void updateComponent(size_t id, std::string name, std::vector<uint8_t> data)
{
    std::cout << "Updating component: " << name << " with ID: " << id << std::endl;
    for (auto u : data)
        std::cout << u << " ";
}

int main() {
    Engine::GameEngine engine(updateComponent);
    ECS::Registry &reg = engine.getRegistry();
    ECS::Entity entity = reg.entityManager().spawnEntity();

    std::vector<std::type_index> types = {
        typeid(Components::Velocity),
        typeid(Components::Position),
        typeid(Components::Controllable)
    };

    try {
        engine.registerComponent<Components::Controllable>("./plugins/bin/components/", "Controllable");
        engine.loadSystems("./plugins/bin/systems/configSystems.cfg");

        std::unique_ptr<Components::Position> position = engine.newComponent<Components::Position>(10, 20, 1);
        std::unique_ptr<Components::Velocity> velocity = engine.newComponent<Components::Velocity>(2, 1);

        reg.componentManager().addComponent<Components::Position>(entity, std::move(position));
        reg.componentManager().addComponent<Components::Velocity>(entity, std::move(velocity));

        boost::asio::io_context io_context;
        UDPServer server(io_context, 8080, engine.getComponents());
        io_context.run();

        displayComponents(reg);

        displayPolymorphic(engine, types.begin(), types.end());

        engine.runSystems();
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
