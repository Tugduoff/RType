#include <SFML/Graphics.hpp>
#include "Window.hpp"

int main()
{
    Window win(1920, 1080, "test");
    while (win.isOpen()) {
        win.clear(sf::Color::Blue);
        win.display();
        while (win.pollEvent());
    }
    return 0;
}
