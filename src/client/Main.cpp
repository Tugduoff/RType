#include <SFML/Graphics.hpp>
#include "Window.hpp"
#include "network/RTypeClient.hpp"
#include "GameEngine/GameEngine.hpp"

void updateComponent(
    [[maybe_unused]]size_t id,
    [[maybe_unused]]std::string name,
    [[maybe_unused]]std::vector<uint8_t> data)
{
}

int main()
{
    Engine::GameEngine engine(updateComponent);
    RTypeClient conn("127.0.0.1", "8080");
    conn.initGame();

    std::unordered_map<uint8_t, std::string> compNames =  conn.getCompNames();
    for (const auto &name : compNames) {
        std::cout << "Commponent n°" << (int)name.first << ": " << name.second << std::endl;
    }

    bool end = false;

    try
    {
        engine.loadSystems("./plugins/bin/systems/configSystems.cfg");
        // Check that you have the same components here 
        while (end != true) {
            // engine.runSystems();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
