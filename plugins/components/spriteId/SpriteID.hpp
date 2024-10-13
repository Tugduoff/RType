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

    enum SpriteID : std::uint8_t {
        Player = 0,
        Enemy,
        ProjectileRight,
        ProjectileLeft,
        Unknown,
    };

    static const std::unordered_map<SpriteID, std::string> SpriteIDToString = {
        {SpriteID::Player, "Player"},
        {SpriteID::Enemy, "Enemy"},
        {SpriteID::ProjectileRight, "ProjectileRight"},
        {SpriteID::ProjectileLeft, "ProjectileLeft"},
        {SpriteID::Unknown, "Unknown"}
    };

    class SpriteIDComponent : public AComponent<SpriteIDComponent> {
    public:
        SpriteIDComponent(SpriteID id = SpriteID::Unknown)
        :   AComponent("SpriteID"),
            id(id)
        {
        }

        SpriteIDComponent(const libconfig::Setting &) : SpriteIDComponent()
        {
            std::cerr << "Building SpriteID from config is not supported yet" << std::endl;
        }

        std::size_t getSize() const override { return 1; }

        virtual std::vector<uint8_t> serialize() override {
            return {static_cast<uint8_t>(id)};
        }

        virtual void deserialize(std::vector<uint8_t> &data) override {
            uint8_t byte = data[0];

            if (byte >= SpriteID::Unknown) {
                id = SpriteID::Unknown;
            } else {
                id = static_cast<SpriteID>(byte);
            }
        }

        virtual void addTo(
            ECS::Entity &to,
            Engine::GameEngine &engine,
            std::vector<std::any> args
        ) override {
            if (args.size() < 1) {
                throw std::runtime_error("Invalid number of arguments for SpriteID component");
            }
            SpriteID id = std::any_cast<SpriteID>(args[0]);
            engine
                .getRegistry()
                .componentManager()
                .addComponent(to, std::make_unique<SpriteIDComponent>(id));
        }

        virtual void addTo(
            ECS::Entity &to,
            Engine::GameEngine &engine,
            libconfig::Setting &config
        ) override {
            int id = 0;

            if (!config.lookupValue("id", id)) {
                std::cerr << "Warning: 'id' not found in config. Using default value: 0\n";
                id = 0;
            }

            std::unique_ptr<Components::SpriteIDComponent> spriteId = engine.newComponent<Components::SpriteIDComponent>(static_cast<uint32_t>(id));
            engine
                .getRegistry()
                .componentManager()
                .addComponent<Components::SpriteIDComponent>(to, std::move(spriteId));
            std::cerr << std::endl;
        }

        SpriteID id;
    };
}
