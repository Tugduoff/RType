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

void updateComponent(size_t id, std::string name, std::vector<uint8_t> data, RTypeClient &conn)
{
    std::vector<uint8_t> updateOperation;

    updateOperation.resize(1 + 2 + 2 + data.size());
    updateOperation[0] = 0x3;
    updateOperation[1] = (uint16_t)id;
    for (const auto &compId : conn.getCompNames()) {
        if (compId.second == name) {
            updateOperation[3] = (uint16_t)compId.first;
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
        // updateComponent
        [&conn](size_t id, std::string name, std::vector<uint8_t> data) { updateComponent(id, name, data, conn); }
    );

    conn.engineInit();
    std::unordered_map<uint8_t, std::string> compNames =  conn.getCompNames();
    for (const auto &name : compNames) {
        std::cout << "Commponent n°" << (int)name.first << ": " << name.second << std::endl;
    }

    try
    {
        engine.loadSystems("./plugins/bin/systems/configSystems.cfg");
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
