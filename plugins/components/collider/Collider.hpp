/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Collider.hpp file
*/

#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "plugins/components/AComponent.hpp"
#include "GameEngine/GameEngine.hpp"
#include <vector>
#include <stdexcept>
#include <cstdint>
#include <libconfig.h++>

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
            appendToData(data, radius);
            appendToData(data, width);
            appendToData(data, height);
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
            if (data.size() != getConstantSize())
                throw std::runtime_error("Invalid data size for Collider component");

            shape = static_cast<ColliderShape>(data[0]);
            radius = *reinterpret_cast<float *>(&data[1]);
            width = *reinterpret_cast<float *>(&data[5]);
            height = *reinterpret_cast<float *>(&data[9]);
        };

        /**
         * @brief Get the fixed size of the serialized data
         * 
         * @return size_t Size in bytes
         */
        size_t getSize() const override {
            return getConstantSize();
        }

        /**
         * @brief Adds the Collider component to an entity.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param args The arguments to pass to the component constructor.
         * 
         * @note This function will add the Collider component to the entity.
         * @note The arguments should be a tuple containing the shape type and dimensions.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() < 1 || args.size() > 3)
                throw std::runtime_error("Invalid number of arguments for Collider component");

            ColliderShape shape = std::any_cast<ColliderShape>(args[0]);
            float radius = 0.0f, width = 0.0f, height = 0.0f;

            if (shape == ColliderShape::Circle && args.size() == 2) {
                radius = std::any_cast<float>(args[1]);
            } else if (shape == ColliderShape::Rectangle && args.size() == 3) {
                width = std::any_cast<float>(args[1]);
                height = std::any_cast<float>(args[2]);
            } else {
                throw std::runtime_error("Invalid arguments for Collider component");
            }

            auto collider = (shape == ColliderShape::Circle) 
                            ? engine.newComponent<Components::Collider>(radius) 
                            : engine.newComponent<Components::Collider>(width, height);
            engine.getRegistry().componentManager().addComponent<Components::Collider>(to, std::move(collider));
        };

        /**
         * @brief Adds the Collider component to an entity from a configuration setting.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param config The configuration setting to extract the component data from.
         * 
         * @note This function will add the Collider component to the entity.
         * @note The configuration setting should contain the keys 'shape', and if the shape is Circle, 'radius', or if Rectangle, 'width' and 'height'.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            int shapeVal;
            if (!config.lookupValue("shape", shapeVal)) {
                throw std::invalid_argument("Failed to retrieve value for 'shape'");
            }

            ColliderShape shape = static_cast<ColliderShape>(shapeVal);
            float radius = 0.0f, width = 0.0f, height = 0.0f;

            if (shape == ColliderShape::Circle) {
                if (!config.lookupValue("radius", radius)) {
                    throw std::invalid_argument("Failed to retrieve value for 'radius'");
                }
                std::unique_ptr<Components::Collider> collider = engine.newComponent<Components::Collider>(radius);
                engine.getRegistry().componentManager().addComponent<Components::Collider>(to, std::move(collider));
            } else if (shape == ColliderShape::Rectangle) {
                if (!config.lookupValue("width", width) || !config.lookupValue("height", height)) {
                    throw std::invalid_argument("Failed to retrieve values for 'width' and 'height'");
                }
                std::unique_ptr<Components::Collider> collider = engine.newComponent<Components::Collider>(width, height);
                engine.getRegistry().componentManager().addComponent<Components::Collider>(to, std::move(collider));
            }
        };

        ColliderShape shape;
        float radius;
        float width;
        float height;

    private:
        /**
         * @brief Returns the constant size of the serialized data.
         * 
         * @return size_t The fixed size of the serialized Collider data.
         */
        size_t getConstantSize() const {
            return sizeof(uint8_t) + 3 * sizeof(float);
        }
        
        /**
         * @brief Helper function to append a float value to a byte vector.
         * 
         * @param data The byte vector to append to.
         * @param value The float value to append.
         */
        void appendToData(std::vector<uint8_t> &data, float value) {
            auto bytes = reinterpret_cast<uint8_t *>(&value);
            data.insert(data.end(), bytes, bytes + sizeof(float));
        }
    };
};

#endif // COLLIDER_HPP
