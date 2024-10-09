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

namespace Components {

    enum SpriteID : std::uint8_t {
        Player = 0,
        Enemy,
        ProjectileRight,
        ProjectileLeft,
        Unknown,
    };

    class SpriteIDComponent : public Components::AComponent {
    public:
        SpriteIDComponent(SpriteID id = SpriteID::Unknown)
        :   Components::AComponent("SpriteId"),
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
            engine
                .getRegistry()
                .componentManager()
                .addComponent(to, std::make_unique<SpriteIDComponent>(config));
        }

        SpriteID id;
    };
}
