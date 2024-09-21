/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** UIButton.hpp file
*/

#include "UIButton.hpp"

UIButton::UIButton(
    sf::Vector2f topLeftCorner,
    sf::Vector2f size,
    std::string text,
    sf::Color color,
    std::optional<std::function<void(IUIElement &)>> onClickCallback,
    std::optional<std::function<void(IUIElement &)>> onHoverCallback,
    size_t zIndex)
    : AUIElement(zIndex, onClickCallback, onHoverCallback),
    __clicked(false),
    __defaultColor(color)
{
    // Create an SFML sprite as the drawable element
    __rectangle = sf::RectangleShape(size);
    __rectangle.setPosition(topLeftCorner);
    __rectangle.setFillColor(color);

    // Create an SFML text as the text element
    __font = sf::Font();
    __font.loadFromFile("assets/fonts/Gendy.otf");

    __text = sf::Text(text, __font, 30);
    __text.setFillColor(sf::Color::White);
    __text.setPosition(topLeftCorner.x + size.x / 2, topLeftCorner.y + size.y / 2 - __text.getGlobalBounds().height / 2);
    __text.setOrigin(__text.getGlobalBounds().width / 2, __text.getGlobalBounds().height / 2);
}

void UIButton::draw(sf::RenderWindow &window) const {
    window.draw(__rectangle);
    window.draw(__text);
}

void UIButton::setColor(sf::Color color) {
    __rectangle.setFillColor(color);
}

bool UIButton::onClick(const sf::Vector2f &mousePosition) {
    if (contains(mousePosition)) {
        __clicked = true;
        // Trigger the click callback if the button contains the click position
        if (_onClickCallback.has_value()) {
            _onClickCallback.value()(*this);
            return true;
        }
    }
    return false;
}

void UIButton::onHover(const sf::Vector2f &mousePosition) {
    if (contains(mousePosition)) {
        // Trigger the hover callback if the button contains the hover position
        if (_onHoverCallback.has_value()) {
            _onHoverCallback.value()(*this);
        }
    }
}

bool UIButton::contains(const sf::Vector2f &point) const {
    // Get the bounds of the button
    sf::FloatRect bounds = __rectangle.getGlobalBounds();
    return bounds.contains(point);
}

bool UIButton::handleEvents(sf::Event &event, sf::Vector2f &worldMousePosition) {
    // Handle events for the button
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            // Check if the button was clicked
            return onClick(worldMousePosition);
        }
    }
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            // Check if the button was focused
            __clicked = false;
            __rectangle.setFillColor(__defaultColor);
            onHover(worldMousePosition);
        }
    }
    if (event.type == sf::Event::MouseMoved) {
        if (__clicked == true)
            return false;
        __rectangle.setScale(sf::Vector2f(1, 1));
        __rectangle.setFillColor(__defaultColor);
        onHover(worldMousePosition);
    }
    return false;
}
