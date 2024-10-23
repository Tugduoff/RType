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
        MainMenuUI() :
            AUI(),
            __cellSize(50),
            __mapSizeX(100),
            __mapSizeY(10),
            __startingZoom(1),
            __start(false) {};
        void initUI() override;

        int handleEvents(sf::Event &event, sf::Vector2f &worldMousePosition) override;

        // Getter methods to retrieve values
        int getCellSize() const { return __cellSize; };
        int getMapSizeX() const { return __mapSizeX; };
        int getMapSizeY() const { return __mapSizeY; };
        int getStartingZoom() const { return __startingZoom; };

    private:

        int __cellSize = 50;
        int __mapSizeX = 100;
        int __mapSizeY = 10;
        int __startingZoom = 1;
        bool __start;
};

#endif // MAINMENUUI_HPP
