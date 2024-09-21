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
        LevelEditorScene(int cellSize, int mapSizeX, int mapSizeY, int startingZoom);
        ~LevelEditorScene() = default;

        void update(sf::RenderWindow &window);

        void render(sf::RenderWindow &window);
        void handleEvents(sf::Event &event, sf::RenderWindow &window);

        sf::View &getView() { return __view; };

    private:

        void createGrid();

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

};

#endif // LEVELEDITORSCENE_HPP
