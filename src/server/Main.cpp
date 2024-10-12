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
#include "plugins/components/controllable/Controllable.hpp"
#include "plugins/components/visible/Visible.hpp"
#include "plugins/components/health/Health.hpp"
#include "plugins/components/collider/Collider.hpp"
#include <exception>
#include <iostream>
#include <typeindex>
#include <vector>

template<typename It>
void displayPolymorphic(Engine::GameEngine &engine, It begin, It end)
{
    int i = 0;

    for (auto it = begin; it != end; ++it) {
        std::type_index &idx = *it;

        std::cout << "For type index {" << idx.name() << "}:\n" << std::endl;

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
                std::cout << (unsigned)byte;
                i++;
            }
            std::cout << "}" << std::endl;
        }
        std::cout << std::endl;
    }
}

void updateComponent(size_t id, std::string name, std::vector<uint8_t> data)
{
    std::cout << "Updating component: " << name << " with ID: " << id << std::endl;
    std::cout << "    {";
    int i = 0;
    for (const auto &byte : data) {
        if (i != 0)
            std::cout << ", ";
        std::cout << (unsigned)byte;
        i++;
    }
    std::cout << "}" << std::endl;
}

int main() {
    Engine::GameEngine engine(updateComponent);
    ECS::Registry &reg = engine.getRegistry();
    ECS::Entity entity = reg.entityManager().spawnEntity();

    std::vector<std::type_index> types = {
        typeid(Components::Velocity),
        typeid(Components::Position),
        typeid(Components::Controllable),
        typeid(Components::Visible),
        typeid(Components::Health),
        typeid(Components::Collider)
    };

    try {
        engine.registerComponent<Components::Controllable>("./plugins/bin/components/", "Controllable");
        engine.registerComponent<Components::Visible>("./plugins/bin/components/", "Visible");
        engine.registerComponent<Components::Health>("./plugins/bin/components/", "Health");
        engine.registerComponent<Components::Collider>("./plugins/bin/components/", "Collider");
        engine.loadSystems("./plugins/bin/systems/configSystems.cfg");

        std::unique_ptr<Components::Position> position = engine.newComponent<Components::Position>(10, 20, 1);
        std::unique_ptr<Components::Velocity> velocity = engine.newComponent<Components::Velocity>(2, 1);

        reg.componentManager().addComponent<Components::Position>(entity, std::move(position));
        reg.componentManager().addComponent<Components::Velocity>(entity, std::move(velocity));

        displayPolymorphic(engine, types.begin(), types.end());

        engine.runSystems();
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
