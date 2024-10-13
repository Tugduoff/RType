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
#include "plugins/components/spriteId/SpriteID.hpp"
#include "plugins/systems/sfml/SpriteComponent.hpp"
#include "plugins/components/controllable/Controllable.hpp"
#include "plugins/components/visible/Visible.hpp"
#include "plugins/components/health/Health.hpp"
#include "plugins/components/collider/Collider.hpp"
#include "plugins/components/acceleration/Acceleration.hpp"

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
    ECS::Registry &reg = engine.getRegistry();
    ECS::Entity entity = reg.entityManager().spawnEntity();

    std::vector<std::type_index> types = {
        typeid(Components::Velocity),
        typeid(Components::Position),
        typeid(Components::Controllable),
        typeid(Components::Visible),
        typeid(Components::Health),
        typeid(Components::Collider),
        typeid(Components::Acceleration)
    };

    try {
        engine.registerComponent<Components::Visible>("./plugins/bin/components/", "Visible");
        engine.registerComponent<Components::Health>("./plugins/bin/components/", "Health");
        engine.registerComponent<Components::Collider>("./plugins/bin/components/", "Collider");

        engine.loadSystems("./plugins/bin/systems/configSystems.cfg");

        while (true) {
            engine.runSystems();
        }
        std::unordered_map<enum Action, enum Key> keyBindings = {
            {Action::FORWARD, Key::Z},
            {Action::BACKWARD, Key::S},
            {Action::LEFT, Key::Q},
            {Action::RIGHT, Key::D},
            {Action::ACTION1, Key::LEFT_CLICK},
            {Action::ACTION2, Key::RIGHT_CLICK},
            {Action::ACTION3, Key::MIDDLE_CLICK},
            {Action::ACTION4, Key::NUM_0},
            {Action::ACTION5, Key::NUM_1},
            {Action::ACTION6, Key::NUM_2},
            {Action::ACTION7, Key::NUM_3},
            {Action::ACTION8, Key::NUM_4},
            {Action::ACTION9, Key::NUM_5},
            {Action::ACTION10, Key::NUM_6}
        };
        std::unique_ptr<Components::SpriteIDComponent> spriteId = engine.newComponent<Components::SpriteIDComponent>(Components::SpriteID::Player);
        std::unique_ptr<Components::SpriteComponent> sprite = std::make_unique<Components::SpriteComponent>();
        std::unique_ptr<Components::Controllable> ctrl = engine.newComponent<Components::Controllable>(keyBindings);
        std::unique_ptr<Components::Velocity> vel = engine.newComponent<Components::Velocity>(0, 0, (uint8_t)90);
        std::unique_ptr<Components::Acceleration> accel = engine.newComponent<Components::Acceleration>(5, -5, 5, -5);
        std::unique_ptr<Components::Position> pos = engine.newComponent<Components::Position>(10, 20, 1);

        reg.componentManager().addComponent<Components::SpriteIDComponent>(entity, std::move(spriteId));
        reg.componentManager().addComponent<Components::SpriteComponent>(entity, std::move(sprite));
        reg.componentManager().addComponent<Components::Controllable>(entity, std::move(ctrl));
        reg.componentManager().addComponent<Components::Velocity>(entity, std::move(vel));
        reg.componentManager().addComponent<Components::Acceleration>(entity, std::move(accel));
        reg.componentManager().addComponent<Components::Position>(entity, std::move(pos));

        engine.getRegistry().componentManager().getComponents<Components::Controllable>()[0]->inputs[(int)Action::FORWARD] = true;

        displayPolymorphic(engine, types.begin(), types.end());

        std::cout << "####################################### iteration 0\n" << std::endl;

        std::string input;
        unsigned int i = 1;
        while (std::getline(std::cin, input)) {
            engine.runSystems();
            displayPolymorphic(engine, types.begin(), types.end());
            std::cout << "####################################### iteration: " << i++ << "\n" << std::endl;
        }

    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
