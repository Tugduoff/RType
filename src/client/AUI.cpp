/*
** EPITECH PROJECT, 2024
** RType - Level Creator
** File description:
** AUI.cpp file
*/

#include "AUI.hpp"

void AUI::addElement(const std::shared_ptr<IUIElement> &element)
{
    __elements.push_back(element);
}

void AUI::removeElement(int index)
{
    __elements.erase(__elements.begin() + index);
}

std::shared_ptr<IUIElement> &AUI::getElement(int index)
{
    return __elements[index];
}

std::vector<std::shared_ptr<IUIElement>> &AUI::getElements()
{
    std::sort(__elements.begin(), __elements.end(), [](const std::shared_ptr<IUIElement> &a, const std::shared_ptr<IUIElement> &b) {
        return a->getZIndex() < b->getZIndex();
    });

    return __elements;
}

int AUI::handleEvents(sf::Event &event, sf::Vector2f &worldMousePosition)
{
    for (auto &element : __elements) {
        if (element->handleEvents(event, worldMousePosition))
            return 2;
    }
    return 0;
}

void AUI::update()
{
    for (auto &element : __elements) {
        element->update();
    }
}

void AUI::render(sf::RenderWindow &window)
{
    std::sort(__elements.begin(), __elements.end(), [](const std::shared_ptr<IUIElement> &a, const std::shared_ptr<IUIElement> &b) {
        return a->getZIndex() < b->getZIndex();
    });

    for (auto &element : __elements) {
        element->draw(window);
    }
}

std::shared_ptr<IUIElement> &AUI::operator[](int index)
{
    return __elements[index];
}

