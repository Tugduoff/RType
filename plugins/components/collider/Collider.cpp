/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Collider.cpp file
*/

#include "Collider.hpp"
#include "library_entrypoint.hpp"

LIBRARY_ENTRYPOINT
Components::IComponent *entryPoint(uint32_t shape, uint32_t radius, uint32_t width, uint32_t height)
{
    if (shape == 0) {
        return new Components::Collider(static_cast<float>(radius));
    } else if (shape == 1) {
        return new Components::Collider(static_cast<float>(width), static_cast<float>(height));
    }
    throw std::invalid_argument("Invalid shape type provided");
}

LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    ColliderShape shape;
    float radius = 0.0f;
    float width = 0.0f;
    float height = 0.0f;

    std::string shapeStr;
    if (config.lookupValue("shape", shapeStr)) {
        if (shapeStr == "Circle") {
            shape = ColliderShape::Circle;
            config.lookupValue("radius", radius);
        } else if (shapeStr == "Rectangle") {
            shape = ColliderShape::Rectangle;
            config.lookupValue("width", width);
            config.lookupValue("height", height);
        } else {
            throw std::invalid_argument("Invalid shape type in config");
        }
    }

    if (shape == ColliderShape::Circle) {
        return new Components::Collider(radius);
    } else if (shape == ColliderShape::Rectangle) {
        return new Components::Collider(width, height);
    }

    throw std::invalid_argument("Failed to create collider from config");
}

LIBRARY_ENTRYPOINT
char const *componentName = "Collider";
