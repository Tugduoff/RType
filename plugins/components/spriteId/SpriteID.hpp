/*
** EPITECH PROJECT, 2024
** RType
** File description:
** SpriteComponent.hpp file
*/

#ifndef SPRITE_ID_COMPONENT_HPP
    #define SPRITE_ID_COMPONENT_HPP

    #include "GameEngine/GameEngine.hpp"
    #include "components/AComponent.hpp"
    #include "components/spriteId/SpriteID.hpp"
    #include "components/scale/Scale.hpp"
    #include "utils/ComponentUtils.hpp"
    #include <libconfig.h++>
    #include <iostream>
    #include <memory>
    #include <string>
    #include <vector>

namespace Components {
    /**
     * @class SpriteID
     * 
     * @brief The SpriteID class is a component that holds the ID of a sprite.
     */
    class SpriteID : public AComponent<SpriteID> {
    public:
        static constexpr std::size_t MAX_ID_SIZE = 20; // Fixed size for network

        SpriteID(const std::string &id = "enemy")
        :   AComponent("SpriteID"),
            id(id)
        {
            std::cerr << "Creating SpriteID component with ID: " << id << std::endl;
            if (this->id.size() > MAX_ID_SIZE) {
                this->id.resize(MAX_ID_SIZE);  // Truncate if longer than 20
            }
        }

        SpriteID(const libconfig::Setting &config) : SpriteID()
        {
            if (!config.lookupValue("id", id)) {
                std::cerr << "Warning: 'id' not found in config. Using default value: enemy\n";
                id = "enemy";
            }
            if (id.size() > MAX_ID_SIZE) {
                id.resize(MAX_ID_SIZE);  // Truncate if longer than 20
            }
        }

        std::size_t getSize() const override { return MAX_ID_SIZE; }

        /**
         * @brief Serializes the sprite ID into a fixed-size vector of 20 bytes.
         *        If the id is shorter than 20 bytes, it will be padded with 0s.
         */
        virtual std::vector<uint8_t> serialize() override {
            std::vector<uint8_t> data(MAX_ID_SIZE, 0);  // Initialize with zeros (20 bytes)
            std::cerr << "Serializing sprite ID: " << id << std::endl;
            std::copy(id.begin(), id.end(), data.begin());  // Copy the string into the vector
            return data;
        }

        /**
         * @brief Deserializes the sprite ID from a fixed-size vector of 20 bytes.
         *        Trailing 0 bytes will be ignored.
         * 
         * @param data A vector of 20 bytes to deserialize into a string.
         */
        virtual void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != MAX_ID_SIZE) {
                throw std::runtime_error("Invalid data size for SpriteID deserialization");
            }
            id = std::string(data.begin(), data.end());
            // Remove trailing null characters (0s)
            id.erase(std::find(id.begin(), id.end(), '\0'), id.end());
            std::cerr << "Deserialized sprite ID: " << id << std::endl;
        }

        virtual void addTo(
            ECS::Entity &to,
            Engine::GameEngine &engine,
            std::vector<std::any> args
        ) override {
            if (args.size() < 1) {
                throw std::runtime_error("Invalid number of arguments for SpriteID component");
            }
            std::string id = std::any_cast<std::string>(args[0]);
            if (id.size() > MAX_ID_SIZE) {
                id.resize(MAX_ID_SIZE);  // Truncate if longer than 20
            }
            engine
                .getRegistry()
                .componentManager()
                .addComponent(to, std::make_unique<SpriteID>(id));
        }

        virtual void addTo(
            ECS::Entity &to,
            Engine::GameEngine &engine,
            libconfig::Setting &config
        ) override {
            std::string spriteId = "enemy";

            if (!config.lookupValue("id", spriteId)) {
                std::cerr << "Warning: 'id' not found in config. Using default value: 'enemy'\n";
            }
            if (spriteId.size() > MAX_ID_SIZE) {
                spriteId.resize(MAX_ID_SIZE);  // Truncate if longer than 20
            }

            std::cerr << "Adding SpriteID component to entity: " << to << " with ID: " << spriteId << std::endl;

            attachAndUpdateComponent<Components::SpriteID>(
                engine, to,
                spriteId
            );
        }

        std::string id;
    };
}

#endif // SPRITE_ID_COMPONENT_HPP
