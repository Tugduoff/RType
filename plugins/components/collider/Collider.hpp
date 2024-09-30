/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Collider.hpp file
*/

#ifndef COLLIDER_HPP
    #define COLLIDER_HPP

    #include "plugins/components/AComponent.hpp"
    #include <vector>
    #include <stdexcept>
    #include <cstdint>

enum class ColliderShape {
    Circle,
    Rectangle
};

namespace Components {

    class Collider : public AComponent {
    public:
        Collider() : shape(ColliderShape::Circle), radius(1.0f), width(0), height(0) {};

        Collider(float radius) : shape(ColliderShape::Circle), radius(radius), width(0), height(0) {};

        Collider(float width, float height) : shape(ColliderShape::Rectangle), radius(0), width(5), height(5) {};

        Collider(float width, float height) : shape(ColliderShape::Rectangle), radius(0), width(width), height(height) {};

        std::vector<uint8_t> serialize() override {
            std::vector<uint8_t> data;
            data.push_back(static_cast<uint8_t>(shape));
            if (shape == ColliderShape::Circle) {
                appendToData(data, radius);
            } else {
                appendToData(data, width);
                appendToData(data, height);
            }
            return data;
        };

        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() < 5)
                throw std::runtime_error("Invalid data size for Collider component");

            shape = static_cast<ColliderShape>(data[0]);

            if (shape == ColliderShape::Circle) {
                radius = *reinterpret_cast<float *>(&data[1]);
            } else {
                width = *reinterpret_cast<float *>(&data[1]);
                height = *reinterpret_cast<float *>(&data[5]);
            }
        };

        size_t getSize() const override {
            if (shape == ColliderShape::Circle) {
                return sizeof(ColliderShape) + sizeof(float);
            } else {
                return sizeof(ColliderShape) + 2 * sizeof(float);
            }
        };

        ColliderShape getShape() const {
            return shape;
        }

        float getRadius() const {
            return radius;
        }

        float getWidth() const {
            return width;
        }

        float getHeight() const {
            return height;
        }

        ColliderShape shape;
        float radius;
        float width;
        float height;

    private:

        void appendToData(std::vector<uint8_t> &data, float value) {
            auto bytes = reinterpret_cast<uint8_t *>(&value);
            data.insert(data.end(), bytes, bytes + sizeof(float));
        }
    };
};

#endif // COLLIDER_HPP
