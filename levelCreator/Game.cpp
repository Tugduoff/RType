/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** Game.cpp file
*/

#include "Game.hpp"

Game::Game() :
    __scene(Scene::MAIN_MENU),
    __window(sf::VideoMode(1280, 800),
    "RType - Level Creator"),
    __ui(std::make_shared<MainMenuUI>()),
    __uiView(sf::FloatRect(0, 0, 1920, 1080))
{
    __ui->initUI();
}

void Game::run()
{
    while (__window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

bool Game::changeScene()
{
    MainMenuUI *mainMenuUI = nullptr;

    switch (__scene) {
        case Scene::MAIN_MENU:
            mainMenuUI = dynamic_cast<MainMenuUI *>(__ui.get());
            if (mainMenuUI) {
                __scene = Scene::LEVEL_EDITOR;
                __ui = std::make_shared<LevelEditorUI>(
                    mainMenuUI->getCellSize(),
                    mainMenuUI->getMapSizeX(),
                    mainMenuUI->getMapSizeY(),
                    mainMenuUI->getStartingZoom()
                );
                __ui->initUI();
                __levelEditorScene = std::make_unique<LevelEditorScene>(
                    mainMenuUI->getCellSize(),
                    mainMenuUI->getMapSizeX(),
                    mainMenuUI->getMapSizeY(),
                    mainMenuUI->getStartingZoom()
                );
            }
            break;

        case Scene::LEVEL_EDITOR:
            return true;

        default:
            break;
    }
    return false;
}

bool Game::checkUIEventsStatus(int status)
{
    if (status == 0)
        return false;
    if (status == 1)
        if (changeScene())
            __window.close();
    if (status == 3)
        __window.close();
    if (status == 4) {
        __scene = Scene::MAIN_MENU;
        __ui = std::make_shared<MainMenuUI>();
        __ui->initUI();
    }
    return true;
}

void Game::handleEvents()
{
    sf::Event event;

    while (__window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            __window.close();
        __window.setView(__uiView);
        sf::Vector2f worldMousePosition = __window.mapPixelToCoords(sf::Mouse::getPosition(__window.getWindow()));
        int status = __ui->handleEvents(event, worldMousePosition);
        if (checkUIEventsStatus(status))
            return;
        if (__scene == Scene::LEVEL_EDITOR)
            __levelEditorScene->handleEvents(event, __window.getWindow());
    }
}

void Game::update()
{
    __window.setView(__uiView);
    __ui->update();
    if (__scene == Scene::LEVEL_EDITOR)
        __levelEditorScene->update(__window.getWindow());
}

void Game::render()
{
    __window.setView(__uiView);
    __window.clear(sf::Color::Black);
    __ui->render(__window.getWindow());
    if (__scene == Scene::LEVEL_EDITOR)
        __levelEditorScene->render(__window.getWindow());
    __window.display();
}
