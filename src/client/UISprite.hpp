/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** UISprite.hpp file
*/

#ifndef UISPRITE_HPP
    #define UISPRITE_HPP

    #include <SFML/Graphics.hpp>
    #include "AUIElement.hpp"
    #include <functional>
    #include <memory>
    #include <optional>

class UISprite : public AUIElement {
    public:
        UISprite(
            sf::Vector2f pos,
            sf::Vector2f scale,
            std::string texturePath,
            std::optional<std::function<void(IUIElement &)>> onClickCallback,
            std::optional<std::function<void(IUIElement &)>> onHoverCallback,
            size_t zIndex);

        void draw(sf::RenderWindow &window) const override;

        bool onClick(const sf::Vector2f &mousePosition) override;
        void onHover(const sf::Vector2f &mousePosition) override;
        bool contains(const sf::Vector2f &point) const override;

        bool handleEvents(sf::Event &event, sf::Vector2f &worldMousePosition) override;
        void update() override {};

        void setScale(sf::Vector2f scale);

    private:

        sf::Vector2f __pos;
        sf::Vector2f __scale;
        sf::Sprite __sprite;
        sf::Texture __texture;
        bool __clicked;
};

#endif // UISPRITE_HPP
