/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** LevelEditorUI.hpp file
*/

#ifndef LEVELEDITORUI_HPP
    #define LEVELEDITORUI_HPP

    #include "AUI.hpp"
    #include "Window.hpp"
    #include <iostream>
    #include <vector>

class LevelEditorUI : public AUI {
    public:
        LevelEditorUI(int cellSize, int mapSizeX, int mapSizeY, int startingZoom) :
            __cellSize(cellSize),
            __mapSizeX(mapSizeX),
            __mapSizeY(mapSizeY),
            __startingZoom(startingZoom),
            __menu(false),
            __preview(false) {};
        void initUI() override;

        int handleEvents(sf::Event &event, sf::Vector2f &worldMousePosition) override;

    private:

        int __cellSize;
        int __mapSizeX;
        int __mapSizeY;
        int __startingZoom;
        bool __menu;
        bool __preview;
};

#endif // LEVELEDITORUI_HPP
