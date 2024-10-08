/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Window.cpp file
*/

#include "Window.hpp"

Window::Window(unsigned int width, unsigned int height, std::string title) : __window(sf::VideoMode(width, height), title)
{
    std::cout << "Creating new window: " << title << std::endl;
}

bool Window::pollEvent() {
    sf::Event event;

    __window.pollEvent(event);
    if (event.type == sf::Event::Closed) {
        close();
        exit(0);
    }
    return false;
};
