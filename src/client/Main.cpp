/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Main.cpp
*/

#include <SFML/Graphics.hpp>
#include "Window.hpp"
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
#include "components/type/Type.hpp"
#include "components/sound/Sound.hpp"
#include "components/destruction/Destruction.hpp"
#include "network/RTypeClient.hpp"
#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>

void displayHelpMessage(bool isError, const char* binary) 
{
    if (isError)
        std::cout << "Error: Invalid usage.\n";

    std::cout << "Usage: " << binary << " [-h hostname] [-p port]\n";
    std::cout << "Options:\n";
    std::cout << "  -h hostname  Specifies the host address\n";
    std::cout << "  -p port      Specifies the port\n";
    std::cout << "  --help       Displays this help message\n";
}

std::pair<std::string, std::string> argumentsHandling(int ac, char **av)
{
    std::string hostname;
    std::string port;
    bool hasHostname = false;
    bool hasPort = false;

    for (int i = 1; i < ac; ++i) {
        if (std::string(av[i]) == "--help") {
            displayHelpMessage(false, av[0]);
            exit(0);
        } else if (std::string(av[i]) == "-h" && i + 1 < ac) {
            hostname = av[i + 1];
            hasHostname = true;
            i++;
        } else if (std::string(av[i]) == "-p" && i + 1 < ac) {
            port = av[i + 1];
            hasPort = true;
            i++;
        }
    }

    if (!hasHostname || !hasPort || ac != 5) {
        displayHelpMessage(true, av[0]);
        exit(84);
    }

    return std::make_pair(hostname, port);
}

int main(int ac, char **av)
{
    auto [hostname, port] = argumentsHandling(ac, av);

    RTypeClient conn(hostname, port);
    Engine::GameEngine engine(
        [&conn](size_t id, std::string name, std::vector<uint8_t> data) { conn.sendUpdateComponent(id, name, data); }
    );
    Chrono chrono;

    engine.loadSystems("./src/client/configClient.cfg");

    conn.menu(&engine);

    conn.asyncReceive(engine);
    // conn.engineInit(engine.getIdStringToType());
    std::unordered_map<uint16_t, std::string> compNames =  conn.getCompNames();
    for (const auto &name : compNames) {
        std::cout << "Commponent n°" << (int)name.first << ": " << name.second << std::endl;
    }

    // Creating thread to start asynchronous operations
    std::thread io_thread([&conn]() {
        conn.getIoContext().run();
    });
    std::thread io_thread2([&conn, &engine]() {
        conn.startInterpret(engine);
    });

    // Starting initialization
    conn.send(std::vector<uint8_t>({0x0}));
    while (!conn.finishedInit)
    {
    }
    conn.send(std::vector<uint8_t>({0x2}));
    conn._compNamesByNetwork.clear();

    try
    {
        // Check that you have the same components here with the map in RTypeClient
        while (conn.gameEnd != true) {
            // std::cout << "Packet queue size: " << conn._packetQueue.size() << std::endl;
            if (!conn.nextFrame) {
                 continue;
            }
            conn.nextFrame = false;
            conn.lockMutex();
            engine.runSystems();
            conn.unlockMutex();
            chrono.restart();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
