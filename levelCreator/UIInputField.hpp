/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** UIInputField.hpp file
*/

#ifndef UIINPUTFIELD_HPP
    #define UIINPUTFIELD_HPP

    #include "AUIElement.hpp"
    #include <SFML/Graphics.hpp>
    #include <functional>
    #include <memory>
    #include <optional>

class UIInputField : public AUIElement {
    public:
        UIInputField(
            sf::Vector2f leftTopCorner,
            sf::Vector2f size,
            std::string initialValue,
            unsigned int maxStringSize,
            std::optional<std::function<void(IUIElement &)>> onClickCallback,
            std::optional<std::function<void(IUIElement &)>> onHoverCallback,
            size_t zIndex);

        void draw(sf::RenderWindow &window) const override;

        bool onClick(const sf::Vector2f &mousePosition) override;
        void onHover(const sf::Vector2f &mousePosition) override;
        bool contains(const sf::Vector2f &point) const override;

        void setColor(sf::Color color);
        void setOutlineColor(sf::Color color) { __rectangle.setOutlineColor(color); };

        bool handleEvents(sf::Event &event, sf::Vector2f &worldMousePosition) override;
        void update() override {};

        std::string &getValue() { return __textString; };

        void reset() { __active = false; };

    private:

        sf::Font __font;
        sf::Text __text;
        sf::RectangleShape __rectangle;
        bool __clicked;
        bool __active;
        std::string __textString;
        unsigned int __maxStringSize;
};

#endif // UIINPUTFIELD_HPP
