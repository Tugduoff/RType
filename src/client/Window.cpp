/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** Window.cpp file
*/

#include "Window.hpp"

Window::Window(sf::VideoMode video_mode, const std::string &title) : __video_mode(video_mode), __window(video_mode, title)
{
}

