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

/**
 * @brief Enum to define different collider shapes
 */
enum class ColliderShape {
    Circle,
    Rectangle
};

namespace Components {

    class Collider : public AComponent {
    public:
        /**
         * @brief Default constructor
         * 
         * Initializes the collider as a circle with a default radius of 1.
         */
        Collider() : shape(ColliderShape::Circle), radius(1.0f), width(0), height(0) {};

        /**
         * @brief Constructor for circle collider
         * 
         * @param radius The radius of the circle.
         */
        Collider(float radius) : shape(ColliderShape::Circle), radius(radius), width(0), height(0) {};

        /**
         * @brief Constructor for rectangle collider
         * 
         * @param width The width of the rectangle.
         * @param height The height of the rectangle.
         */
        Collider(float width, float height) : shape(ColliderShape::Rectangle), radius(0), width(width), height(height) {};

        /**
         * @brief Serialize the collider data
         * 
         * Serializes the shape, radius, width, and height into a byte vector for transmission or storage.
         * 
         * @return std::vector<uint8_t> Serialized data.
         */
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

        /**
         * @brief Deserialize the collider data
         * 
         * Deserializes the shape, radius, width, and height from the provided byte vector.
         * 
         * @param data The byte vector containing serialized data.
         * @throws std::runtime_error If the data size is invalid.
         */
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

        /**
         * @brief Get the size of the serialized data
         * 
         * @return size_t Size in bytes
         */
        size_t getSize() const override {
            if (shape == ColliderShape::Circle) {
                return sizeof(ColliderShape) + sizeof(float);
            } else {
                return sizeof(ColliderShape) + 2 * sizeof(float);
            }
        };

        /**
         * @brief Get the shape type of the collider.
         * 
         * @return ColliderShape The shape of the collider.
         */
        ColliderShape getShape() const {
            return shape;
        }

        /**
         * @brief Get the radius (for circle colliders).
         * 
         * @return float The radius of the circle.
         */
        float getRadius() const {
            return radius;
        }

        /**
         * @brief Get the width (for rectangle colliders).
         * 
         * @return float The width of the rectangle.
         */
        float getWidth() const {
            return width;
        }

        /**
         * @brief Get the height (for rectangle colliders).
         * 
         * @return float The height of the rectangle.
         */
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
