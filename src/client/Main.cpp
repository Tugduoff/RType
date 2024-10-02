#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Entity System");
    while (1)
    {
        window.display();
    }
    window.close();
    return 0;
}
