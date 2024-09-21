/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** LevelEditorScene.cpp file
*/

#include "LevelEditorScene.hpp"

LevelEditorScene::LevelEditorScene(int cellSize, int mapSizeX, int mapSizeY, int startingZoom) :
    __isDragging(false),
    __lastMousePosition(0, 0),
    __zoomFactor((float)startingZoom),
    __view(sf::FloatRect(0, 0, 1920, 1080)),
    __gridSizeX(mapSizeX),
    __gridSizeY(mapSizeY),
    __cellSize(cellSize),
    __gridWidth(__gridSizeX * __cellSize),
    __gridHeight(__gridSizeY * __cellSize)
{
    createGrid();
    __view.setCenter(__gridWidth / 2, __gridHeight / 2);
}

void LevelEditorScene::update(sf::RenderWindow &window)
{
    if (__isDragging) {
        sf::Vector2f currentMousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Vector2f offset = __lastMousePosition - currentMousePosition;

        __view.move(offset * __zoomFactor);
        __lastMousePosition = currentMousePosition;
    }
}

void LevelEditorScene::render(sf::RenderWindow &window)
{
    window.setView(__view);
    window.draw(__grid);
}

void LevelEditorScene::handleEvents(sf::Event &event, sf::RenderWindow &window)
{
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            __isDragging = true;
            __lastMousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        }
    }
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            __isDragging = false;
        }
    }
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.delta > 0) {
            __view.zoom(1 - 0.1f);
            __zoomFactor *= (1 - 0.1f);
        } else {
            __view.zoom(1 + 0.1f);
            __zoomFactor *= (1 + 0.1f);
        }
        window.setView(__view);
    }
}

void LevelEditorScene::createGrid() {
    __grid = sf::VertexArray(sf::Lines);

    // Vertical lines
    for (int i = 0; i <= __gridSizeX; ++i) {
        float x = i * __cellSize;
        __grid.append(sf::Vertex(sf::Vector2f(x, 0), sf::Color::White));
        __grid.append(sf::Vertex(sf::Vector2f(x, __gridHeight), sf::Color::White));
    }

    // Horizontal lines
    for (int i = 0; i <= __gridSizeY; ++i) {
        float y = i * __cellSize;
        __grid.append(sf::Vertex(sf::Vector2f(0, y), sf::Color::White));
        __grid.append(sf::Vertex(sf::Vector2f(__gridWidth, y), sf::Color::White));
    }
}
