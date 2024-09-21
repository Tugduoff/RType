/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** MainMenuUI.hpp file
*/

#ifndef MAINMENUUI_HPP
    #define MAINMENUUI_HPP

    #include "AUI.hpp"
    #include "Window.hpp"
    #include <iostream>
    #include <vector>

class MainMenuUI : public AUI {
    public:
        MainMenuUI() = default;
        void initUI() override;

        int handleEvents(sf::Event &event, sf::Vector2f &worldMousePosition) override;

        // Getter methods to retrieve values
        int getCellSize() const { return __cellSize; };
        int getMapSizeX() const { return __mapSizeX; };
        int getMapSizeY() const { return __mapSizeY; };
        int getStartingZoom() const { return __startingZoom; };

    private:

        int __cellSize;
        int __mapSizeX;
        int __mapSizeY;
        int __startingZoom;
        bool __start;
};

#endif // MAINMENUUI_HPP
