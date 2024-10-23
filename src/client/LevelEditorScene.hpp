/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** LevelEditorScene.hpp file
*/

#ifndef LEVELEDITORSCENE_HPP
    #define LEVELEDITORSCENE_HPP

    #include <SFML/Graphics.hpp>

class LevelEditorScene {
    public:
        LevelEditorScene(int cellSize = 50, int mapSizeX = 100, int mapSizeY = 10, int startingZoom = 1);
        ~LevelEditorScene() = default;

        void update(sf::RenderWindow &window);

        void render(sf::RenderWindow &window);
        void handleEvents(sf::Event &event, sf::RenderWindow &window);

        sf::View &getView() { return __view; };

    private:

        void createGrid();
        void createWindowGrid(int minHeight, int maxHeight);

        bool __isDragging;
        sf::Vector2f __lastMousePosition;
        float __zoomFactor;
        sf::View __view;
        const int __gridSizeX;
        const int __gridSizeY;
        const int __cellSize;
        const int __gridWidth;
        const int __gridHeight;
        sf::VertexArray __grid;
        sf::VertexArray __gridBig;
        sf::Sprite __playerSprite;
        sf::Texture __playerTexture;
        sf::VertexArray __windowOutline;
        sf::VertexArray __windowGrid;
};

#endif // LEVELEDITORSCENE_HPP
