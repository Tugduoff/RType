/*
** EPITECH PROJECT, 2024
** ConfigEditor
** File description:
** Main.cpp file
*/

#include "Gui.hpp"

int main()
{
    ConfigEditor::Gui gui;

    while (!gui.shouldExit()) {
        gui.tick();
    }
    gui.close();
}
