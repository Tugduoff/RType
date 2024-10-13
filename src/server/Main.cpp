/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Main.cpp file
*/

#include "GameEngine/GameEngine.hpp"
#include "ECS/registry/Registry.hpp"
#include "ECS/utilities/SparseArray.hpp"
#include "Chrono.hpp"

#include "plugins/components/IComponent.hpp"
#include "plugins/components/position/Position.hpp"
#include "plugins/components/velocity/Velocity.hpp"
#include "plugins/components/spriteId/SpriteID.hpp"
#include "plugins/components/controllable/Controllable.hpp"
#include "plugins/components/visible/Visible.hpp"
#include "plugins/components/health/Health.hpp"
#include "plugins/components/collider/Collider.hpp"
#include "plugins/components/acceleration/Acceleration.hpp"
#include "plugins/components/gun/Gun.hpp"
#include "plugins/components/damage/Damage.hpp"

#include <exception>
#include <iostream>
#include <typeindex>
#include <vector>

template<typename It>
void displayPolymorphic(Engine::GameEngine &engine, It begin, It end)
{
    int i = 0;

    std::cout << std::endl;
    for (auto it = begin; it != end; ++it) {
        std::type_index &idx = *it;

        std::cout << "For type index {" << idx.name() << "}:\n" << std::endl;

        auto &dummyComp = engine.getComponentFromId(idx);

        auto &sparseArray = dummyComp->any_cast(
            engine.getRegistry().componentManager().getComponents(idx)
        );

        for (auto const &comp : sparseArray) {
            if (!comp)
                continue;
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
    Chrono chrono;

    std::vector<std::type_index> types = {
        typeid(Components::Velocity),
        typeid(Components::Position),
        typeid(Components::Controllable),
        typeid(Components::Visible),
        typeid(Components::Health),
        typeid(Components::Collider),
        typeid(Components::Acceleration),
        typeid(Components::Gun),
        typeid(Components::Damage)
    };

    try {
        engine.registerComponent<Components::Visible>("./plugins/bin/components/", "Visible");
        engine.registerComponent<Components::Health>("./plugins/bin/components/", "Health");
        engine.registerComponent<Components::Collider>("./plugins/bin/components/", "Collider");

        engine.loadSystems("./plugins/bin/systems/configSystems.cfg");

        displayPolymorphic(engine, types.begin(), types.end());

        std::cout << "####################################### iteration 0\n" << std::endl;

        unsigned int i = 1;
        while (true) {
            if (chrono.getElapsedTime() < 17)
                continue;
            engine.runSystems();
            displayPolymorphic(engine, types.begin(), types.end());
            std::cout << "####################################### iteration: " << i++ << "\n" << std::endl;
            chrono.restart();
       }

    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
