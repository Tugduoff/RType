/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** LevelEditorScene.cpp file
*/

#include "LevelEditorScene.hpp"
#include <iostream>

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
    std::cout << "LevelEditorScene created" << std::endl;
    std::cout << "Grid size: x " << __gridSizeX << " y " << __gridSizeY << std::endl;
    std::cout << "Cell size: " << __cellSize << std::endl;
    std::cout << "Grid width: " << __gridWidth << std::endl;
    std::cout << "Grid height: " << __gridHeight << std::endl;
    createGrid();
    __view.setCenter(__gridWidth / 2, __gridHeight / 2);

    __playerTexture = sf::Texture();
    __playerTexture.loadFromFile("assets/spacecraft.png");

    __playerSprite = sf::Sprite(__playerTexture);
    __playerSprite.setScale(1, 1);
    __playerSprite.setPosition(0, __gridHeight / 2);
    __playerSprite.setOrigin(__playerSprite.getGlobalBounds().width / 2, __playerSprite.getGlobalBounds().height / 2);

    int minHeight = __gridHeight < 1080 ? __gridHeight / 2 - 540 : 0;
    int maxHeight = __gridHeight < 1080 ? __gridHeight / 2 + 540 : 1080;
    __windowOutline = sf::VertexArray(sf::Lines);
    __windowOutline.append(sf::Vertex(sf::Vector2f(0, minHeight), sf::Color(0, 0, 255, 150)));
    __windowOutline.append(sf::Vertex(sf::Vector2f(1920, minHeight), sf::Color(0, 0, 255, 150)));
    __windowOutline.append(sf::Vertex(sf::Vector2f(1920, minHeight), sf::Color(0, 0, 255, 150)));
    __windowOutline.append(sf::Vertex(sf::Vector2f(1920, maxHeight), sf::Color(0, 0, 255, 150)));
    __windowOutline.append(sf::Vertex(sf::Vector2f(1920, maxHeight), sf::Color(0, 0, 255, 150)));
    __windowOutline.append(sf::Vertex(sf::Vector2f(0, maxHeight), sf::Color(0, 0, 255, 150)));
    __windowOutline.append(sf::Vertex(sf::Vector2f(0, maxHeight), sf::Color(0, 0, 255, 150)));
    __windowOutline.append(sf::Vertex(sf::Vector2f(0, minHeight), sf::Color(0, 0, 255, 150)));
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
    // window.draw(__windowGrid);
    window.draw(__windowOutline);
    if (__zoomFactor < 0.5 * __cellSize / 10)
        window.draw(__grid);
    window.draw(__gridBig);
    window.draw(__playerSprite);
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
        float zoomFactor = (event.mouseWheelScroll.delta > 0) ? 0.9f : 1.1f;
        float newZoomFactor = __zoomFactor * zoomFactor;

        if (newZoomFactor >= 0.1f && newZoomFactor <= 10.0f) {
            __view.zoom(zoomFactor);
            __zoomFactor = newZoomFactor;
        }
        window.setView(__view);
    }
}

void LevelEditorScene::createGrid()
{
    __grid = sf::VertexArray(sf::Lines);
    __gridBig = sf::VertexArray(sf::Lines);

    // Vertical lines
    for (int i = 1; i <= __gridSizeX; ++i) {
        float x = i * __cellSize;
        __grid.append(sf::Vertex(sf::Vector2f(x, 0), sf::Color(255, 255, 255, 50)));
        __grid.append(sf::Vertex(sf::Vector2f(x, __gridHeight), sf::Color(255, 255, 255, 50)));
        if (i % 10 == 0) {
            __gridBig.append(sf::Vertex(sf::Vector2f(x, 0), sf::Color(255, 0, 0, 150)));
            __gridBig.append(sf::Vertex(sf::Vector2f(x, __gridHeight), sf::Color(255, 0, 0, 150)));
        }
    }

    // Horizontal lines
    for (int i = 0; i <= __gridSizeY; ++i) {
        float y = i * __cellSize;
        __grid.append(sf::Vertex(sf::Vector2f(0, y), sf::Color(255, 255, 255, 50)));
        __grid.append(sf::Vertex(sf::Vector2f(__gridWidth, y), sf::Color(255, 255, 255, 50)));
        if (i % 10 == 0) {
            __gridBig.append(sf::Vertex(sf::Vector2f(0, y), sf::Color(255, 0, 0, 150)));
            __gridBig.append(sf::Vertex(sf::Vector2f(__gridWidth, y), sf::Color(255, 0, 0, 150)));
        }
    }
}

void LevelEditorScene::createWindowGrid(int minHeight, int maxHeight)
{
    __windowGrid = sf::VertexArray(sf::Lines);

    // Vertical lines
    for (int i = 0; i <= 1920; i += 100) {
        __windowGrid.append(sf::Vertex(sf::Vector2f(i, minHeight), sf::Color(200, 200, 200, 50)));
        __windowGrid.append(sf::Vertex(sf::Vector2f(i, maxHeight), sf::Color(200, 200, 200, 50)));
    }

    // Horizontal lines
    for (int i = minHeight; i <= maxHeight; i += 100) {
        __windowGrid.append(sf::Vertex(sf::Vector2f(0, i), sf::Color(200, 200, 200, 50)));
        __windowGrid.append(sf::Vertex(sf::Vector2f(1920, i), sf::Color(200, 200, 200, 50)));
    }
}
