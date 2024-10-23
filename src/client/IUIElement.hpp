/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** IUIElement.hpp file
*/

// This class represents an UI Element in the level creator
// It is the interface for all UI elements

#ifndef IUIELEMENT_HPP
    #define IUIELEMENT_HPP

    #include <SFML/Graphics.hpp>
    #include <string>

class IUIElement {
    public:
        virtual ~IUIElement() = default;

        // Rendering and Drawable
        virtual void draw(sf::RenderWindow &window) const = 0;

        // Event handling
        virtual bool onClick(const sf::Vector2f &mousePosition) = 0;
        virtual void onHover(const sf::Vector2f &mousePosition) = 0;
        virtual bool contains(const sf::Vector2f &point) const = 0;

        virtual bool handleEvents(sf::Event &event, sf::Vector2f &worldMousePosition) = 0;
        virtual void update() = 0;

        virtual size_t getZIndex() const = 0;
};

#endif // IUIELEMENT_HPP
