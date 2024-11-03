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


int main()
{
    RTypeClient conn("127.0.0.1", "8080");
    Engine::GameEngine engine(
        [&conn](size_t id, std::string name, std::vector<uint8_t> data) { conn.sendUpdateComponent(id, name, data); }
    );
    Chrono chrono;

    engine.loadSystems("./src/client/configClient.cfg");

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
