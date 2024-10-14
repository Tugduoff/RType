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
#include "Chrono.hpp"

#include "network/UDPServer.hpp"

#include "components/IComponent.hpp"
#include "components/position/Position.hpp"
#include "components/velocity/Velocity.hpp"
#include "components/spriteId/SpriteID.hpp"
#include "components/controllable/Controllable.hpp"
#include "components/visible/Visible.hpp"
#include "components/health/Health.hpp"
#include "components/collider/Collider.hpp"
#include "components/acceleration/Acceleration.hpp"
#include "components/gun/Gun.hpp"
#include "components/damage/Damage.hpp"
#include "components/scale/Scale.hpp"
#include "components/deathRange/DeathRange.hpp"

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
        typeid(Components::Damage),
        typeid(Components::DeathRange),
        typeid(Components::Scale),
    };

    try {
        engine.registerComponent<Components::Visible>("./plugins/bin/components/", "Visible");
        engine.registerComponent<Components::Health>("./plugins/bin/components/", "Health");
        engine.registerComponent<Components::Collider>("./plugins/bin/components/", "Collider");

        engine.loadSystems("./src/server/configServer.cfg");

        // we'll probably have to move it elsewhere
        boost::asio::io_context io_context;
        UDPServer server(io_context, 8080, engine.getIdStringToType());
        while(server.client_endpoints.empty())
            server.start_receive();

        displayPolymorphic(engine, types.begin(), types.end());

        std::cout << "####################################### iteration 0\n" << std::endl;

        unsigned int i = 1;
        while (true) {
            server.start_receive();
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
