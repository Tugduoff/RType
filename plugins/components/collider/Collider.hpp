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

namespace Components {

    class Collider : public AComponent<Collider> {
    public:
        /**
         * @brief Default constructor
         * 
         * Initializes the rectangle collider with default width and height of 0.
         */
        Collider(float width = 1.0f, float height = 1.0f) : AComponent(std::string("Collider")), width(width), height(height) {};

        /**
         * @brief Constructor using configuration.
         * 
         * Initializes the rectangle collider based on config settings, with default values
         * if lookupValue fails.
         */
        Collider(libconfig::Setting &config) : AComponent(std::string("Collider")) {
            if (!config.lookupValue("width", width)) {
                width = 1.0f;
            }
            if (!config.lookupValue("height", height)) {
                height = 1.0f;
            }
        }        

        /**
         * @brief Serialize the collider data
         * 
         * Serializes the width and height into a byte vector for transmission or storage.
         * 
         * @return std::vector<uint8_t> Serialized data.
         */
        std::vector<uint8_t> serialize() override {
            std::vector<uint8_t> data;
            appendToData(data, width);
            appendToData(data, height);
            return data;
        };

        /**
         * @brief Deserialize the collider data
         * 
         * Deserializes the width and height from the provided byte vector.
         * 
         * @param data The byte vector containing serialized data.
         * @throws std::runtime_error If the data size is invalid.
         */
        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != getConstantSize())
                throw std::runtime_error("Invalid data size for Collider component");

            width = *reinterpret_cast<float *>(&data[0]);
            height = *reinterpret_cast<float *>(&data[sizeof(float)]);
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
         * @note The arguments should be a tuple containing the width and height.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() != 2)
                throw std::runtime_error("Invalid number of arguments for Collider component");

            float width = std::any_cast<float>(args[0]);
            float height = std::any_cast<float>(args[1]);

            auto collider = engine.newComponent<Components::Collider>(width, height);
            engine.getRegistry().componentManager().addComponent<Components::Collider>(to, std::move(collider));
        };

        /**
         * @brief Adds the Collider component to an entity from a configuration setting.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param config The configuration setting to extract the component data from.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
        float width = 0.0f;
        float height = 0.0f;

        if (!config.lookupValue("width", width)) {
            std::cerr << "Warning: 'width' not found in config. Using default value: 1.0f\n";
            width = 1.0f;
        }
        
        if (!config.lookupValue("height", height)) {
            std::cerr << "Warning: 'height' not found in config. Using default value: 1.0f\n";
            height = 1.0f;
        }
        std::unique_ptr<Components::Collider> collider = engine.newComponent<Components::Collider>(width, height);
        engine.getRegistry().componentManager().addComponent<Components::Collider>(to, std::move(collider));
    }


        float width;
        float height;
        char const *componentName;

    private:
        /**
         * @brief Returns the constant size of the serialized data.
         * 
         * @return size_t The fixed size of the serialized Collider data.
         */
        size_t getConstantSize() const {
            return 2 * sizeof(float);
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
