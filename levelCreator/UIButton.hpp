/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** UIButton.hpp file
*/

#ifndef UIBUTTON_HPP
    #define UIBUTTON_HPP

    #include "AUIElement.hpp"
    #include <SFML/Graphics.hpp>
    #include <functional>
    #include <memory>
    #include <optional>

class UIButton : public AUIElement {
    public:
        UIButton(
            sf::Vector2f leftTopCorner,
            sf::Vector2f size,
            std::string text,
            sf::Color color,
            std::optional<std::function<void(IUIElement &)>> onClickCallback,
            std::optional<std::function<void(IUIElement &)>> onHoverCallback,
            size_t zIndex);

        void draw(sf::RenderWindow &window) const override;

        bool onClick(const sf::Vector2f &mousePosition) override;
        void onHover(const sf::Vector2f &mousePosition) override;
        bool contains(const sf::Vector2f &point) const override;

        void setColor(sf::Color color);
        void setScale(sf::Vector2f scale) { __rectangle.setScale(scale); };

        bool handleEvents(sf::Event &event, sf::Vector2f &worldMousePosition) override;
        void update() override {};

    private:

        sf::RectangleShape __rectangle;
        bool __clicked;
        sf::Font __font;
        sf::Text __text;
        sf::Color __defaultColor;

};

#endif // UIBUTTON_HPP
