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
#include "components/type/Type.hpp"
#include "components/sound/Sound.hpp"
#include "components/destruction/Destruction.hpp"
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

    if (!hasHostname || !hasPort || ac != 4) {
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
    // Hard coded register for now
    engine.registerComponent<Components::Visible>("./plugins/bin/components/", "Visible");
    engine.registerComponent<Components::Health>("./plugins/bin/components/", "Health");
    engine.registerComponent<Components::Collider>("./plugins/bin/components/", "Collider");
    engine.registerComponent<Components::Velocity>("./plugins/bin/components/", "Velocity");
    engine.registerComponent<Components::Acceleration>("./plugins/bin/components/", "Acceleration");
    engine.registerComponent<Components::Damage>("./plugins/bin/components/", "Damage");
    engine.registerComponent<Components::Controllable>("./plugins/bin/components/", "Controllable");
    engine.registerComponent<Components::Gun>("./plugins/bin/components/", "Gun");
    engine.registerComponent<Components::Type>("./plugins/bin/components/", "Type");
    engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position");
    engine.registerComponent<Components::SpriteID>("./plugins/bin/components/", "SpriteID");
    engine.registerComponent<Components::DeathRange>("./plugins/bin/components/", "DeathRange");
    engine.registerComponent<Components::Sound>("./plugins/bin/components/", "Sound");
    engine.registerComponent<Components::Destruction>("./plugins/bin/components/", "Destruction");

    conn.setEngine(&engine);
    conn.engineInit();
    std::unordered_map<uint8_t, std::string> compNames =  conn.getCompNames();
    for (const auto &name : compNames) {
        std::cout << "Commponent n°" << (int)name.first << ": " << name.second << std::endl;
    }

    conn.asyncReceive(engine);
    std::thread io_thread([&conn]() {
        conn.getIoContext().run();
    });
    try
    {
        // Check that you have the same components here with the map in RTypeClient
        while (conn.gameEnd != true) {
            if (chrono.getElapsedTime() < 17)
                continue;
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
