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
#include "ECS/entity/Entity.hpp"
#include "GameEngine/GameEngine.hpp"
#include "ECS/registry/Registry.hpp"
#include "utils/Chrono.hpp"

#include "components/IComponent.hpp"
#include "components/position/Position.hpp"
#include "components/velocity/Velocity.hpp"
#include "components/controllable/Controllable.hpp"
#include "components/visible/Visible.hpp"
#include "components/health/Health.hpp"
#include "components/collider/Collider.hpp"
#include "components/acceleration/Acceleration.hpp"
#include "components/gun/Gun.hpp"
#include "components/damage/Damage.hpp"
#include "components/scale/Scale.hpp"
#include "components/deathRange/DeathRange.hpp"
#include "components/sound/Sound.hpp"
#include "components/destruction/Destruction.hpp"
#include "components/Ai/Ai.hpp"
#include "components/type/Type.hpp"
#include "components/spriteId/SpriteID.hpp"
#include "components/modelId/ModelId.hpp"

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
    Engine::GameEngine engine(
        []([[maybe_unused]]size_t id, [[maybe_unused]]std::string name, [[maybe_unused]]std::vector<uint8_t> data) {  }
    );
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
        typeid(Components::Damage),
        typeid(Components::DeathRange),
        typeid(Components::Ai),
        // typeid(Components::Scale),
    };

    try {
        engine.registerComponent<Components::Visible>("./plugins/bin/components/", "Visible");
        // engine.registerComponent<Components::Velocity>("./plugins/bin/components/", "Velocity");
        engine.registerComponent<Components::Health>("./plugins/bin/components/", "Health");
        engine.registerComponent<Components::Collider>("./plugins/bin/components/", "Collider");
        engine.registerComponent<Components::Scale>("./plugins/bin/components/", "Scale");
        engine.registerComponent<Components::Sound>("./plugins/bin/components/", "Sound");
        engine.registerComponent<Components::Destruction>("./plugins/bin/components/", "Destruction");
        engine.registerComponent<Components::ModelId>("./plugins/bin/components/", "ModelId");

        engine.loadSystems("./src/server/configServer.cfg");

        // displayPolymorphic(engine, types.begin(), types.end());
        // ECS::Entity entity = engine.getRegistry().createEntity();
        // std::cerr << "New entity created with ID: " << entity << std::endl;

        // srand(time(NULL));
        // int posY = rand() % 1080;

        // attachAndUpdateComponent<Components::Position>(engine, entity, 50, posY, 2);
        // attachAndUpdateComponent<Components::Velocity>(engine, entity, 0, 0, 100);
        // attachAndUpdateComponent<Components::Collider>(engine, entity, 30, 30);
        // attachAndUpdateComponent<Components::Damage>(engine, entity, 50);
        // attachAndUpdateComponent<Components::Type>(engine, entity, Components::TypeID::ALLY);
        // attachAndUpdateComponent<Components::SpriteID>(engine, entity, "player");
        // attachAndUpdateComponent<Components::Acceleration>(engine, entity, -5, 5, -5, 5);
        // attachAndUpdateComponent<Components::Gun>(engine, entity, 50, 500, 8, 0, "shot1");
        // attachAndUpdateComponent<Components::Scale>(engine, entity, 300, 300);
        // attachAndUpdateComponent<Components::Health>(engine, entity, 100);

        std::map<enum Action, enum Key> keyBindings = {
            {Action::FORWARD, Key::UNKNOWN},
            {Action::BACKWARD, Key::UNKNOWN},
            {Action::LEFT, Key::UNKNOWN},
            {Action::RIGHT, Key::UNKNOWN},
            {Action::ACTION1, Key::UNKNOWN},
            {Action::ACTION2, Key::UNKNOWN},
            {Action::ACTION3, Key::UNKNOWN},
            {Action::ACTION4, Key::UNKNOWN},
            {Action::ACTION5, Key::UNKNOWN},
            {Action::ACTION6, Key::UNKNOWN},
            {Action::ACTION7, Key::UNKNOWN},
            {Action::ACTION8, Key::UNKNOWN},
            {Action::ACTION9, Key::UNKNOWN},
            {Action::ACTION10, Key::UNKNOWN}
        };

        std::unique_ptr<Components::Controllable> component = std::make_unique<Components::Controllable>(keyBindings);
        std::vector<uint8_t> data = component->serialize();

        // engine.getRegistry().componentManager().addComponent(entity, std::move(component));

        while (true) {
            if (chrono.getElapsedTime() < 17)
                continue;
            engine.runSystems();
            chrono.restart();
       }

    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
