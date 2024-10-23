/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** UI.hpp file
*/

// This class represents the UI of the level creator

#ifndef IUI_HPP
    #define IUI_HPP

    #include <iostream>
    #include <vector>
    #include "IUIElement.hpp"
    #include "Window.hpp"

class IUI {
    public:

        virtual ~IUI() = default;

        virtual void initUI() = 0;
        virtual std::shared_ptr<IUIElement> &operator[](int index) = 0;
        virtual void addElement(const std::shared_ptr<IUIElement> &element) = 0;
        virtual void removeElement(int index) = 0;
        virtual std::shared_ptr<IUIElement> &getElement(int index) = 0;
        virtual std::vector<std::shared_ptr<IUIElement>> &getElements() = 0;

        virtual int handleEvents(sf::Event &event, sf::Vector2f &worldMousePosition) = 0;

        virtual void update() = 0;
        virtual void render(sf::RenderWindow &window) = 0;
};

#endif // IUI_HPP
