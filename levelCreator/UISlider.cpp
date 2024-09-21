/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** UISlider.cpp file
*/

#include "UISlider.hpp"

UISlider::UISlider(
    sf::Vector2f leftTopCorner,
    sf::Vector2f size,
    int minValue,
    int value,
    int maxValue,
    sf::Color color,
    std::optional<std::function<void(IUIElement &)>> onClickCallback,
    std::optional<std::function<void(IUIElement &)>> onHoverCallback,
    size_t zIndex) :
    AUIElement(zIndex, onClickCallback, onHoverCallback),
    __minValue(minValue),
    __value(value),
    __maxValue(maxValue),
    __clicked(false),
    __defaultColor(color)
{
    int radius = size.y * 1.5f;
    __body = sf::CircleShape(radius);
    __body.setOrigin(radius, radius - size.y / 2);
    __body.setFillColor(color);
    int valueOffset = (size.x * (value - minValue)) / (maxValue - minValue);
    __body.setPosition(leftTopCorner.x + valueOffset, leftTopCorner.y);
    __slider = sf::RectangleShape(size);
    __slider.setPosition(leftTopCorner.x, leftTopCorner.y);
    __slider.setFillColor(sf::Color::White);

    __font = sf::Font();
    __font.loadFromFile("assets/fonts/Tarragon.otf");

    __valueText = sf::Text(std::to_string(value), __font, 30);
    __valueText.setFillColor(sf::Color::White);
    __valueText.setPosition(leftTopCorner.x + size.x + 30, leftTopCorner.y - size.y / 2 - 10);
}

void UISlider::draw(sf::RenderWindow &window) const {
    window.draw(__slider);
    window.draw(__body);
    window.draw(__valueText);
}

bool UISlider::onClick(const sf::Vector2f &mousePosition) {
    if (contains(mousePosition)) {
        __clicked = true;
        __body.setPosition(sf::Vector2f(mousePosition.x, __body.getPosition().y));
        if (_onClickCallback.has_value()) {
            _onClickCallback.value()(*this);
            return true;
        }
    }
    if (__slider.getGlobalBounds().contains(mousePosition)) {
        __clicked = true;
        __body.setPosition(sf::Vector2f(mousePosition.x, __body.getPosition().y));
        __value = __minValue + ((__body.getPosition().x - __slider.getPosition().x) / __slider.getSize().x) * (__maxValue - __minValue);
        __valueText.setString(std::to_string(__value));
        if (_onClickCallback.has_value()) {
            _onClickCallback.value()(*this);
            return true;
        }
    }
    return false;
}

void UISlider::onHover(const sf::Vector2f &mousePosition) {
    if (contains(mousePosition)) {
        if (_onHoverCallback.has_value()) {
            _onHoverCallback.value()(*this);
        }
    }
}

bool UISlider::contains(const sf::Vector2f &point) const {
    return __body.getGlobalBounds().contains(point);
}

bool UISlider::handleEvents(sf::Event &event, sf::Vector2f &worldMousePosition) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            return onClick(worldMousePosition);
        }
    }
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            __clicked = false;
            __body.setFillColor(__defaultColor);
            onHover(worldMousePosition);
        }
    }
    if (event.type == sf::Event::MouseMoved) {
        if (__clicked == true) {
            __body.setPosition(sf::Vector2f(worldMousePosition.x, __body.getPosition().y));
            if (__body.getPosition().x < __slider.getPosition().x) {
                __body.setPosition(__slider.getPosition().x, __body.getPosition().y);
            }
            if (__body.getPosition().x > __slider.getPosition().x + __slider.getSize().x) {
                __body.setPosition(__slider.getPosition().x + __slider.getSize().x, __body.getPosition().y);
            }
            __value = __minValue + ((__body.getPosition().x - __slider.getPosition().x) / __slider.getSize().x) * (__maxValue - __minValue);
            __valueText.setString(std::to_string(__value));
            return true;
        }
        __body.setFillColor(__defaultColor);
        onHover(worldMousePosition);
    }
    return false;
}

void UISlider::setColor(sf::Color color) {
    __body.setFillColor(color);
}
