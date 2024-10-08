#include <SFML/Graphics.hpp>
#include "Window.hpp"
#include "Network/RTypeClient.hpp"

int main()
{
    Window win(1920, 1080, "test");

    RTypeClient conn("127.0.0.1", "8080");

    std::string message = "Hello from client!";

    conn.send(message);
    std::cout << "Sent message: " << UDPConnection::binaryToStr(UDPConnection::strToBinary(message)) << std::endl;

    std::unordered_map<uint8_t, std::string> compNames = conn.initGame();
    for (const auto &name : compNames) {
        std::cout << "Commponent n°" << name.first << ": " << name.second << std::endl;
    }

    while (win.isOpen()) {
        win.clear(sf::Color::Blue);
        win.display();
        while (win.pollEvent());
    }
    return 0;
}
