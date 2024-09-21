/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** UIInputField.cpp file
*/

#include "UIInputField.hpp"
#include <iostream>

UIInputField::UIInputField(
    sf::Vector2f leftTopCorner,
    sf::Vector2f size,
    std::string initialValue,
    unsigned int maxStringSize,
    std::optional<std::function<void(IUIElement &)>> onClickCallback,
    std::optional<std::function<void(IUIElement &)>> onHoverCallback,
    size_t zIndex) :
    AUIElement(zIndex, onClickCallback, onHoverCallback),
    __clicked(false),
    __active(false),
    __textString(initialValue),
    __maxStringSize(maxStringSize)
{
    __font = sf::Font();
    __font.loadFromFile("assets/fonts/Gendy.otf");

    __text = sf::Text(__textString, __font, 30);
    __text.setFillColor(sf::Color::White);
    __text.setPosition(leftTopCorner.x + size.x / 2, leftTopCorner.y + size.y / 2 - __text.getGlobalBounds().height / 2);
    __text.setOrigin(__text.getGlobalBounds().width / 2,
        __text.getGlobalBounds().height / 2);
    
    __rectangle = sf::RectangleShape(size);
    __rectangle.setPosition(leftTopCorner);
    __rectangle.setFillColor(sf::Color(150, 150, 150, 255));
    __rectangle.setOutlineColor(sf::Color(200, 200, 200, 255));
    __rectangle.setOutlineThickness(5);
}

void UIInputField::draw(sf::RenderWindow &window) const
{
    window.draw(__rectangle);
    window.draw(__text);
}

bool UIInputField::onClick(const sf::Vector2f &mousePos)
{
    if (contains(mousePos)) {
        __clicked = true;
        __active = true;
        if (_onClickCallback.has_value()) {
            _onClickCallback.value()(*this);
            return true;
        }
    }
    __active = false;
    __rectangle.setOutlineColor(sf::Color(200, 200, 200, 255));
    return false;
}

void UIInputField::onHover(const sf::Vector2f &mousePos)
{
    if (contains(mousePos)) {
        if (_onHoverCallback.has_value()) {
            _onHoverCallback.value()(*this);
        }
    }
}

bool UIInputField::contains(const sf::Vector2f &point) const
{
    return __rectangle.getGlobalBounds().contains(point);
}

bool UIInputField::handleEvents(sf::Event &event, sf::Vector2f &worldMousePos)
{
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            return onClick(worldMousePos);
        }
    }
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            __clicked = false;
        }
    }
    if (event.type == sf::Event::TextEntered) {
        if (__active) {
            if (event.text.unicode == 8 && __textString.size() > 0) {
                __textString.pop_back();
                if (__textString.size() == 0) {
                    __textString = "0";
                }
            } else if (event.text.unicode < 128 && __textString.size() < __maxStringSize) {
                if (__textString == "0")
                    __textString = "";
                __textString += event.text.unicode;
                if (_onClickCallback.has_value()) {
                    _onClickCallback.value()(*this);
                }
            }
            __text.setString(__textString);
            __text.setOrigin(__text.getGlobalBounds().width / 2,
                __text.getGlobalBounds().height / 2);
        }
    }
    return false;
}

void UIInputField::setColor(sf::Color color)
{
    __rectangle.setFillColor(color);
}
