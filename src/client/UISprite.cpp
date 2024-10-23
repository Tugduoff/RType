/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** UISprite.cpp file
*/

#include "UISprite.hpp"
#include <iostream>

UISprite::UISprite(
    sf::Vector2f pos,
    sf::Vector2f scale,
    std::string texturePath,
    std::optional<std::function<void(IUIElement &)>> onClickCallback,
    std::optional<std::function<void(IUIElement &)>> onHoverCallback,
    size_t zIndex)
    : AUIElement(zIndex, onClickCallback, onHoverCallback),
    __pos(pos),
    __scale(scale),
    __sprite(),
    __texture()
{
    int x = 0;
    int y = 0;

    __texture.loadFromFile(texturePath);
    __sprite.setTexture(__texture);
    x = __sprite.getTexture()->getSize().x;
    y = __sprite.getTexture()->getSize().y;
    __sprite.setOrigin(x / 2, y / 2);
    __sprite.setPosition(__pos);
    __sprite.setScale(scale);
}

void UISprite::draw(sf::RenderWindow &window) const {
    window.draw(__sprite);
}

bool UISprite::onClick(const sf::Vector2f &mousePosition) {
    if (contains(mousePosition)) {
        __clicked = true;
        if (_onClickCallback.has_value()) {
            _onClickCallback.value()(*this);
            return true;
        }
    }
    return false;
}

void UISprite::onHover(const sf::Vector2f &mousePosition) {
    if (contains(mousePosition)) {
        if (_onHoverCallback.has_value()) {
            _onHoverCallback.value()(*this);
        }
    }
}

bool UISprite::contains(const sf::Vector2f &point) const {
    sf::FloatRect bounds = __sprite.getGlobalBounds();
    return bounds.contains(point);
}

bool UISprite::handleEvents(sf::Event &event, sf::Vector2f &worldMousePosition) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            return onClick(worldMousePosition);
        }
    }
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            __clicked = false;
            // Nothing there yet
        }
    }
    if (event.type == sf::Event::MouseMoved) {
        if (__clicked == true)
            return false;
        __sprite.setPosition(__pos);
        __sprite.setScale(__scale);
        onHover(worldMousePosition);
    }
    return false;
}

void UISprite::setScale(sf::Vector2f scale) {
    __sprite.setScale(scale);
}
