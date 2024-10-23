/*
** EPITECH PROJECT, 2024
** RType
** File description:
** SpriteComponent.hpp file
*/

#include "GameEngine/GameEngine.hpp"
#include "components/AComponent.hpp"
#include <libconfig.h++>
#include <any>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <sys/types.h>
#include <unordered_map>

namespace Components {
    /**
     * @class SpriteID
     * 
     * @brief The SpriteID class is a component that holds the ID of a sprite.
     */
    class SpriteID : public AComponent<SpriteID> {
    public:
        SpriteID(const std::string &id = "enemy")
        :   AComponent("SpriteID"),
            id(id)
        {
        }

        SpriteID(const libconfig::Setting &config) : SpriteID()
        {
            if (!config.lookupValue("id", id)) {
                std::cerr << "Warning: 'id' not found in config. Using default value: enemy\n";
                id = "enemy";
            }
        }

        std::size_t getSize() const override { return 1; }

        virtual std::vector<uint8_t> serialize() override {
            std::vector<uint8_t> data(id.begin(), id.end());
            return data;
        }

        /**
         * @brief Deserializes the sprite ID from a vector of bytes.
         * 
         * @param data A vector of bytes to deserialize into a string.
         */
        virtual void deserialize(std::vector<uint8_t> &data) override {
            id = std::string(data.begin(), data.end());
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

            std::unique_ptr<Components::SpriteID> spriteIdComponent =
                engine.newComponent<Components::SpriteID>(spriteId);
            engine
                .getRegistry()
                .componentManager()
                .addComponent<Components::SpriteID>(to, std::move(spriteIdComponent));
        }

        std::string id;
    };
}
