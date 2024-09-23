/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** MainMenuUI.hpp file
*/

#include "MainMenuUI.hpp"
#include "UIButton.hpp"
#include "UISprite.hpp"
#include "UISlider.hpp"
#include "UIInputField.hpp"
#include <optional>
#include <functional>

void MainMenuUI::initUI()
{
    addElement(std::make_shared<UISlider>(
        sf::Vector2f(200, 450),
        sf::Vector2f(640, 10),
        2,
        50,
        100,
        sf::Color(150, 150, 150, 255),
        std::function<void(IUIElement &)>([this](IUIElement &element) {
            UISlider &slider = dynamic_cast<UISlider &>(element);
            slider.setColor(sf::Color::Blue);
            __cellSize = slider.getValue();
        }),
        std::function<void(IUIElement &)>([this](IUIElement &element) {
            UISlider &slider = dynamic_cast<UISlider &>(element);
            slider.setColor(sf::Color::Red);
            __cellSize = slider.getValue();
        }),
        2));
    addElement(std::make_shared<UISprite>(
        sf::Vector2f(1920 / 2, 1080 / 2),
        sf::Vector2f(1, 1),
        "./assets/bg.jpg",
        std::nullopt,
        std::nullopt,
        0));
    addElement(std::make_shared<UISprite>(
        sf::Vector2f(550, 650),
        sf::Vector2f(1, 1),
        "./assets/box.png",
        std::nullopt,
        std::function<void(IUIElement &)>([](IUIElement &element) {
            UISprite &sprite = dynamic_cast<UISprite &>(element);
            sprite.setScale(sf::Vector2f(1.02, 1.02));
        }),
        1));
    addElement(std::make_shared<UISprite>(
        sf::Vector2f(550, 200),
        sf::Vector2f(1, 1),
        "./assets/title.png",
        std::nullopt,
        std::function<void(IUIElement &)>([](IUIElement &element) {
            UISprite &sprite = dynamic_cast<UISprite &>(element);
            sprite.setScale(sf::Vector2f(1.05, 1.05));
        }),
        2));
    addElement(std::make_shared<UISprite>(
        sf::Vector2f(1920 / 2, 1050),
        sf::Vector2f(1, 1),
        "./assets/version.png",
        std::nullopt,
        std::function<void(IUIElement &)>([](IUIElement &element) {
            UISprite &sprite = dynamic_cast<UISprite &>(element);
            sprite.setScale(sf::Vector2f(1.1, 1.1));
        }),
        1));
    addElement(std::make_shared<UISprite>(
        sf::Vector2f(270, 390),
        sf::Vector2f(1, 1),
        "./assets/cellSize.png",
        std::nullopt,
        std::function<void(IUIElement &)>([](IUIElement &element) {
            UISprite &sprite = dynamic_cast<UISprite &>(element);
            sprite.setScale(sf::Vector2f(1.1, 1.1));
        }),
        2));
    addElement(std::make_shared<UIInputField>(
        sf::Vector2f(200, 570),
        sf::Vector2f(270, 50),
        "100",
        3,
        std::function<void(IUIElement &)>([this](IUIElement &element) {
            UIInputField &inputField = dynamic_cast<UIInputField &>(element);
            inputField.setOutlineColor(sf::Color::Cyan);
            __mapSizeX = atoi(inputField.getValue().c_str());
        }),
        std::nullopt,
        2));
    addElement(std::make_shared<UISprite>(
        sf::Vector2f(520, 595),
        sf::Vector2f(1, 1),
        "./assets/x.png",
        std::nullopt,
        std::function<void(IUIElement &)>([](IUIElement &element) {
            UISprite &sprite = dynamic_cast<UISprite &>(element);
            sprite.setScale(sf::Vector2f(1.1, 1.1));
        }),
        2));
    addElement(std::make_shared<UIInputField>(
        sf::Vector2f(570, 570),
        sf::Vector2f(270, 50),
        "10",
        3,
        std::function<void(IUIElement &)>([this](IUIElement &element) {
            UIInputField &inputField = dynamic_cast<UIInputField &>(element);
            inputField.setOutlineColor(sf::Color::Cyan);
            __mapSizeY = atoi(inputField.getValue().c_str());
        }),
        std::nullopt,
        2));
    addElement(std::make_shared<UISprite>(
        sf::Vector2f(890, 595),
        sf::Vector2f(1, 1),
        "./assets/y.png",
        std::nullopt,
        std::function<void(IUIElement &)>([](IUIElement &element) {
            UISprite &sprite = dynamic_cast<UISprite &>(element);
            sprite.setScale(sf::Vector2f(1.1, 1.1));
        }),
        2));
    addElement(std::make_shared<UISprite>(
        sf::Vector2f(270, 520),
        sf::Vector2f(1, 1),
        "./assets/mapSize.png",
        std::nullopt,
        std::function<void(IUIElement &)>([](IUIElement &element) {
            UISprite &sprite = dynamic_cast<UISprite &>(element);
            sprite.setScale(sf::Vector2f(1.1, 1.1));
        }),
        2));
    addElement(std::make_shared<UISprite>(
        sf::Vector2f(310, 680),
        sf::Vector2f(1, 1),
        "./assets/startingZoom.png",
        std::nullopt,
        std::function<void(IUIElement &)>([](IUIElement &element) {
            UISprite &sprite = dynamic_cast<UISprite &>(element);
            sprite.setScale(sf::Vector2f(1.1, 1.1));
        }),
        2));
    addElement(std::make_shared<UISlider>(
        sf::Vector2f(200, 740),
        sf::Vector2f(640, 10),
        1,
        1,
        10,
        sf::Color(150, 150, 150, 255),
        std::function<void(IUIElement &)>([this](IUIElement &element) {
            UISlider &slider = dynamic_cast<UISlider &>(element);
            slider.setColor(sf::Color::Blue);
            __startingZoom = slider.getValue();
        }),
        std::function<void(IUIElement &)>([this](IUIElement &element) {
            UISlider &slider = dynamic_cast<UISlider &>(element);
            slider.setColor(sf::Color::Red);
            __startingZoom = slider.getValue();
        }),
        2));
    addElement(std::make_shared<UIButton>(
        sf::Vector2f(200, 830),
        sf::Vector2f(700, 100),
        "Create",
        sf::Color(150, 150, 150, 255),
        std::function<void(IUIElement &)>([this](IUIElement &element) {
            UIButton &button = dynamic_cast<UIButton &>(element);
            button.setColor(sf::Color(100, 100, 100, 255));
            __start = true;
        }),
        std::function<void(IUIElement &)>([](IUIElement &element) {
            UIButton &button = dynamic_cast<UIButton &>(element);
            button.setColor(sf::Color(125, 125, 125, 255));
        }),
        2));
}

int MainMenuUI::handleEvents(sf::Event &event, sf::Vector2f &worldMousePosition)
{
    int status = 0;

    std::vector<sf::Keyboard::Key> keys({
        sf::Keyboard::Numpad0,
        sf::Keyboard::Numpad1,
        sf::Keyboard::Numpad2,
        sf::Keyboard::Numpad3,
        sf::Keyboard::Numpad4,
        sf::Keyboard::Numpad5,
        sf::Keyboard::Numpad6,
        sf::Keyboard::Numpad7,
        sf::Keyboard::Numpad8,
        sf::Keyboard::Numpad9,
        sf::Keyboard::BackSpace
    });

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::L)
            return 3;
        for (auto &key : keys) {
            if (event.key.code == key)
                return 0;
        }
    }
    for (auto &element : getElements()) {
        if (element->handleEvents(event, worldMousePosition))
            status = 2;
    }
    if (__start) {
        status = 1;
        __start = false;
    }
    return status;
}
