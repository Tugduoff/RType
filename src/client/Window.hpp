/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** Window.hpp file
*/

// This class is a wrapper around the SFML window class

#ifndef WINDOW_HPP
    #define WINDOW_HPP

    #include <SFML/Graphics.hpp>
    #include <iostream>
    #include <memory>

class Window {
    public: 

        Window(sf::VideoMode video_mode, const std::string &title);
        Window(const Window &window) = delete;

        Window &operator=(const Window &window) = delete;

        ~Window() = default;

        bool isOpen() const { return __window.isOpen(); }

        void clear(const sf::Color &color = sf::Color::Black) { __window.clear(color); }
        void display() { __window.display(); }
        void close() { __window.close(); }
        bool pollEvent(sf::Event &event) { return __window.pollEvent(event); }
        void setView(const sf::View &view) { __window.setView(view); }
        void draw(const sf::Drawable &drawable) { __window.draw(drawable); }

        sf::Vector2f mapPixelToCoords(const sf::Vector2i &point) { return __window.mapPixelToCoords(point); }

        sf::RenderWindow &getWindow() { return __window; }

    private:

        sf::VideoMode __video_mode;
        sf::RenderWindow __window;

};

#endif // WINDOW_HPP
