/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** AUIElement.hpp file
*/

#ifndef AUIELEMENT_HPP
    #define AUIELEMENT_HPP

    #include "IUIElement.hpp"
    #include <SFML/Graphics.hpp>
    #include <functional>
    #include <memory>
    #include <optional>

class AUIElement : public IUIElement {
    public:
        AUIElement(
            size_t zIndex,
            std::optional<std::function<void(IUIElement &)>> onClickCallback,
            std::optional<std::function<void(IUIElement &)>> onHoverCallback) :
            _zIndex(zIndex),
            _onClickCallback(onClickCallback),
            _onHoverCallback(onHoverCallback) {};
        virtual ~AUIElement() = default;

        // Delete copy operations, as managing unique pointers
        AUIElement(const AUIElement &) = delete;
        AUIElement &operator=(const AUIElement &) = delete;

        // Drawable management (implemented in abstract class)
        virtual void draw(sf::RenderWindow &window) const = 0;

        // Basic event handling (click event)
        virtual bool onClick(const sf::Vector2f &mousePosition) = 0;
        virtual void onHover(const sf::Vector2f &mousePosition) = 0;
        virtual bool contains(const sf::Vector2f &point) const = 0;

        virtual bool handleEvents(sf::Event &event, sf::Vector2f &worldMousePosition) = 0;
        virtual void update() = 0;

        virtual size_t getZIndex() const { return _zIndex; };

    protected:

        size_t _zIndex;
        std::optional<std::function<void(IUIElement &)>> _onClickCallback;
        std::optional<std::function<void(IUIElement &)>> _onHoverCallback;
};

#endif // AUIELEMENT_HPP
