/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** LevelEditorUI.hpp file
*/

#include "LevelEditorUI.hpp"
#include "UIButton.hpp"
#include "UISprite.hpp"
#include <optional>
#include <functional>

void LevelEditorUI::initUI()
{
    addElement(std::make_shared<UISprite>(
        sf::Vector2f(1920 / 2, 1080 / 2),
        sf::Vector2f(1, 1),
        "./assets/bg2.png",
        std::nullopt,
        std::nullopt,
        0));
    addElement(std::make_shared<UIButton>(
        sf::Vector2f(50, 50),
        sf::Vector2f(200, 50),
        "Menu",
        sf::Color(150, 150, 150, 255),
        std::function<void(IUIElement &)>([this](IUIElement &element) {
            UIButton &button = dynamic_cast<UIButton &>(element);
            button.setColor(sf::Color(100, 100, 100, 255));
            __menu = true;
        }),
        std::function<void(IUIElement &)>([](IUIElement &element) {
            UIButton &button = dynamic_cast<UIButton &>(element);
            button.setColor(sf::Color(125, 125, 125, 255));
        }),
        2));
    addElement(std::make_shared<UIButton>(
        sf::Vector2f(50, 150),
        sf::Vector2f(200, 50),
        "Preview",
        sf::Color(150, 150, 150, 255),
        std::function<void(IUIElement &)>([this](IUIElement &element) {
            UIButton &button = dynamic_cast<UIButton &>(element);
            button.setColor(sf::Color(100, 100, 100, 255));
            __preview = true;
        }),
        std::function<void(IUIElement &)>([](IUIElement &element) {
            UIButton &button = dynamic_cast<UIButton &>(element);
            button.setColor(sf::Color(125, 125, 125, 255));
        }),
        2));
}

int LevelEditorUI::handleEvents(sf::Event &event, sf::Vector2f &worldMousePosition)
{
    if (event.type == sf::Event::KeyPressed)
        if (event.key.code == sf::Keyboard::L)
            return 1;
    for (auto &element : getElements()) {
        if (element->handleEvents(event, worldMousePosition))
            return 2;
    }
    if (__menu)
        return 4;
    if (__preview)
        return 5;
    return 0;
}
