/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Main.cpp
*/

#include <SFML/Graphics.hpp>
#include "Window.hpp"
#include "network/RTypeClient.hpp"
#include "GameEngine/GameEngine.hpp"
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

void updateComponent(size_t id, std::string name, std::vector<uint8_t> data, RTypeClient &conn)
{
    std::vector<uint8_t> updateOperation;

    updateOperation.resize(1 + 2 + 2 + data.size());
    updateOperation[0] = 0x3;
    updateOperation[1] = (uint16_t)id;
    for (const auto &compId : conn.getCompNames()) {
        if (compId.second == name) {
            updateOperation[3] = (uint16_t)compId.first;
            break;
        }
    }
    // add a verification that a compId matched with name
    updateOperation.insert(updateOperation.begin() + 5, data.begin(), data.end());
    conn.send(updateOperation);
}

int main()
{
    RTypeClient conn("127.0.0.1", "8080");
    Engine::GameEngine engine(
        [&conn](size_t id, std::string name, std::vector<uint8_t> data) { updateComponent(id, name, data, conn); }
    );

    // Hard coded register for now
    engine.registerComponent<Components::Visible>("./plugins/bin/components/", "Visible");
    engine.registerComponent<Components::Health>("./plugins/bin/components/", "Health");
    engine.registerComponent<Components::Collider>("./plugins/bin/components/", "Collider");
    engine.registerComponent<Components::Velocity>("./plugins/bin/components/", "Velocity");
    engine.registerComponent<Components::Acceleration>("./plugins/bin/components/", "Acceleration");


    conn.engineInit();
    std::unordered_map<uint8_t, std::string> compNames =  conn.getCompNames();
    for (const auto &name : compNames) {
        std::cout << "Commponent n°" << (int)name.first << ": " << name.second << std::endl;
    }

    try
    {
        engine.loadSystems("./src/client/configClient.cfg");
        // Check that you have the same components here with the map in RTypeClient
        while (conn.gameEnd != true) {
            if (conn.dataFromServer()) {
                conn.interpretServerData(engine);
            }
            engine.runSystems();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
