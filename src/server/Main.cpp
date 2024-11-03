/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Main.cpp file
*/


#include <exception>
#include <iostream>
#include <thread>
#include <typeindex>
#include <vector>
#include "ECS/entity/Entity.hpp"
#include "network/UDPServer.hpp"
#include "GameEngine/GameEngine.hpp"
#include "ECS/registry/Registry.hpp"
#include "utils/Chrono.hpp"

#include "network/UDPServer.hpp"

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
    Engine::GameEngine engine(
        []([[maybe_unused]]size_t id, [[maybe_unused]]std::string name, [[maybe_unused]]std::vector<uint8_t> data) { std::cout << "Empty update component" << std::endl; }
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
        // we'll probably have to move it elsewhere
        boost::asio::io_context io_context;
        UDPServer server(
            io_context,
            8080,
            engine.getIdStringToType(),
            [&engine]() -> const std::vector<ECS::Entity> & {
                return engine.getRegistry().entityManager().viewEntities();
            },
            ComponentsGetter(engine)
        );

        engine.getRegistry().addEntityCreateCallback(
            [&server](const ECS::Entity &e) { server.create_entity(e); }
        );
        engine.getRegistry().addEntityKillCallback(
            [&server](const ECS::Entity &e) { server.delete_entity(e); }
        );

        // engine.getRegistry().componentManager().registerGlobalCreateCallback(
        //     [&server](std::type_index type, size_t index) { server.attach_component(index, type); }
        // );
        engine.getRegistry().componentManager().registerGlobalRemoveCallback(
            [&server](std::type_index type, size_t index) { server.detach_component(index, type); }
        );

        engine.setUpdateComponent(
            [&server](size_t id, std::string name, std::vector<uint8_t> data) { server.update_component(id, name, data); }
        );

        engine.registerComponent<Components::Visible>("./plugins/bin/components/", "Visible");
        engine.registerComponent<Components::Health>("./plugins/bin/components/", "Health");
        engine.registerComponent<Components::Collider>("./plugins/bin/components/", "Collider");
        engine.registerComponent<Components::Scale>("./plugins/bin/components/", "Scale");
        engine.registerComponent<Components::Sound>("./plugins/bin/components/", "Sound");
        engine.registerComponent<Components::Destruction>("./plugins/bin/components/", "Destruction");

        engine.loadSystems("./src/server/configServer.cfg");

        server.updateIdStringToType(engine.getIdStringToType());

        // engine.getRegistry().componentManager().
        server.start_receive(engine);
        std::thread io_thread([&io_context]() { io_context.run(); });
        while(!server.gameRunning()) {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1ms);
        }

        std::thread io_thread1([&io_context]() { io_context.run(); });
        std::thread io_thread2([&io_context]() { io_context.run(); });
        std::thread io_thread3([&io_context]() { io_context.run(); });
        std::thread io_thread4([&io_context]() { io_context.run(); });

        displayPolymorphic(engine, types.begin(), types.end());

        std::cout << "####################################### iteration 0\n" << std::endl;

        unsigned int i = 1;
        while (true) {
            if (chrono.getElapsedTime() < 17)
                continue;
            server.lockMutex();
            engine.runSystems();
            server.unlockMutex();
            server.sendNextFrame();
            // displayPolymorphic(engine, types.begin(), types.end());
            std::cout << "####################################### iteration: " << i++ << "\n" << std::endl;
            chrono.restart();
       }

    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
