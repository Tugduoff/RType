/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** UISlider.hpp file
*/

#ifndef UISLIDER_HPP
    #define UISLIDER_HPP

    #include "AUIElement.hpp"
    #include <SFML/Graphics.hpp>
    #include <functional>
    #include <memory>
    #include <optional>

class UISlider : public AUIElement {
    public:
        UISlider(
            sf::Vector2f leftTopCorner,
            sf::Vector2f size,
            int minValue,
            int value,
            int maxValue,
            sf::Color color,
            std::optional<std::function<void(IUIElement &)>> onClickCallback,
            std::optional<std::function<void(IUIElement &)>> onHoverCallback,
            size_t zIndex);

        void draw(sf::RenderWindow &window) const override;

        bool onClick(const sf::Vector2f &mousePosition) override;
        void onHover(const sf::Vector2f &mousePosition) override;
        bool contains(const sf::Vector2f &point) const override;

        void setColor(sf::Color color);
        int &getValue() { return __value; };

        bool handleEvents(sf::Event &event, sf::Vector2f &worldMousePosition) override;
        void update() override {};

    private:

        sf::CircleShape __body;
        sf::RectangleShape __slider;
        int __minValue;
        int __value;
        int __maxValue;
        bool __clicked;
        sf::Color __defaultColor;
        sf::Text __valueText;
        sf::Font __font;
};

#endif // UISLIDER_HPP
