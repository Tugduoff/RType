/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** AUI.hpp file
*/

// This class represents the UI of the level creator

#ifndef AUI_HPP
    #define AUI_HPP

    #include "IUIElement.hpp"
    #include "Window.hpp"
    #include "IUI.hpp"
    #include <iostream>
    #include <vector>

class AUI : public IUI {
    public:
        AUI() = default;
        AUI(const AUI &ui) = delete;

        AUI &operator=(const AUI &ui) = delete;

        ~AUI() = default;

        virtual void initUI() = 0;
        std::shared_ptr<IUIElement> &operator[](int index) override;
        void addElement(const std::shared_ptr<IUIElement> &element) override;
        void removeElement(int index) override;
        std::shared_ptr<IUIElement> &getElement(int index) override;
        std::vector<std::shared_ptr<IUIElement>> &getElements() override;

        int handleEvents(sf::Event &event, sf::Vector2f &worldMousePosition) override;

        void update() override;
        void render(sf::RenderWindow &window) override;
    private:

        std::vector<std::shared_ptr<IUIElement>> __elements;
};

#endif // AUI_HPP
