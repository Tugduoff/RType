#include <SFML/Graphics.hpp>
#include "Window.hpp"
#include "network/RTypeClient.hpp"
#include "GameEngine/GameEngine.hpp"

int main()
{
    Engine::GameEngine engine;
    RTypeClient conn("127.0.0.1", "8080");
    conn.initGame();

    std::unordered_map<uint8_t, std::string> compNames =  conn.getCompNames();
    for (const auto &name : compNames) {
        std::cout << "Commponent n°" << (int)name.first << ": " << name.second << std::endl;
    }

    bool end = false;

    try
    {
        // engine.loadSystems("./plugins/bin/systems/configSystems.cfg");
        // // Check that you have the same components here 
        // while (end != true) {
        //     engine.runSystems();
        // }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
