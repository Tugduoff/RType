/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** Game.hpp file
*/

#ifndef GAME_HPP
    #define GAME_HPP

    #include <SFML/Graphics.hpp>
    #include "Window.hpp"
    #include "MainMenuUI.hpp"
    #include "LevelEditorUI.hpp"
    #include "LevelEditorScene.hpp"

class Game {
    public:
        enum Scene {
            MAIN_MENU,
            LEVEL_EDITOR
        };

        Game();
        ~Game() = default;

        void run();

    private:

        void handleEvents();
        void update();
        void render();

        bool changeScene();
        bool checkUIEventsStatus(int status);

        Scene __scene;
        Window __window;
        std::shared_ptr<IUI> __ui;
        std::unique_ptr<LevelEditorScene> __levelEditorScene;
        sf::View __uiView;

};

#endif // GAME_HPP