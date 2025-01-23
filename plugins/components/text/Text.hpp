/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Text.hpp file
*/

#ifndef TEXT_HPP
    #define TEXT_HPP

    #include "GameEngine/GameEngine.hpp"
    #include "components/AComponent.hpp"
    #include "utils/ComponentUtils.hpp"
    #include <libconfig.h++>
    #include <iostream>
    #include <memory>
    #include <string>
    #include <vector>

namespace Components {
    /**
     * @class Text
     * 
     * @brief The Text class is a component that holds the ID of a model.
     */

    enum class TextID : uint8_t {
        SCORE = 0,
        UNKNOWN = 255 // Optional: Handle an invalid state
    };

    class Text : public AComponent<Text> {
    public:
        static constexpr std::size_t MAX_TEXT_SIZE = 50; // Fixed size for network

        Text(const std::string &value = "", const TextID id = TextID::SCORE)
        :   AComponent("Text"),
            value(value), id(id)
        {
            std::cerr << "Creating Text component with value: " << value << std::endl;
        }

        Text(const libconfig::Setting &config) : Text()
        {
            int idInt = 0;

            if (!config.lookupValue("value", value)) {
                value = "";
            }
            if (!config.lookupValue("id", idInt)) {
                id = TextID::UNKNOWN;
            } else {
                id = static_cast<TextID>(idInt);
            }
        }

        std::size_t getSize() const override { return MAX_TEXT_SIZE + sizeof(TextID); }

        /**
         * @brief Serializes the sprite ID into a fixed-size vector of 20 bytes.
         *        If the value is shorter than 20 bytes, it will be padded with 0s.
         */
        virtual std::vector<uint8_t> serialize() override {
            // no need
        }

        /**
         * @brief Deserializes the sprite ID from a fixed-size vector of 20 bytes.
         *        Trailing 0 bytes will be ignored.
         * 
         * @param data A vector of 20 bytes to deserialize into a string.
         */
        virtual void deserialize(std::vector<uint8_t> &data) override {
            // no need
        }

        virtual void addTo(
            ECS::Entity &to,
            Engine::GameEngine &engine,
            std::vector<std::any> args
        ) override {
            if (args.size() != 2)
                throw std::runtime_error("Invalid number of arguments for Text component");

            std::string modelId = std::any_cast<std::string>(args[0]);
            uint32_t idInt = std::any_cast<uint32_t>(args[1]);

            attachAndUpdateComponent<Components::Text>(
                engine, to,
                modelId,
                static_cast<TextID>(idInt)
            );
        }

        virtual void addTo(
            ECS::Entity &to,
            Engine::GameEngine &engine,
            libconfig::Setting &config
        ) override {
            std::string modelId = "";
            uint32_t idInt = 0;

            if (!config.lookupValue("value", modelId)) {
                std::cerr << "Warning: 'value' not found in config. Make sure to fill it.\n";
            }

            if (!config.lookupValue("id", idInt)) {
                std::cerr << "Warning: 'id' not found in config. Make sure to fill it.\n";
            }

            std::cerr << "Adding Text component to entity: " << to << " with value: " << modelId << std::endl;

            attachAndUpdateComponent<Components::Text>(
                engine, to,
                modelId,
                static_cast<TextID>(idInt)
            );
        }

        std::string value;
        TextID id;
    };
}

#endif // TEXT_HPP
