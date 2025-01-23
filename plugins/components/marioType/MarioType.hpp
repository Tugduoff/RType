/*
** EPITECH PROJECT, 2024
** RType
** File description:
** MarioType.hpp file
*/

#ifndef MARIO_TYPE_HPP
    #define MARIO_TYPE_HPP

    #include "GameEngine/GameEngine.hpp"
    #include "components/AComponent.hpp"
    #include "utils/ComponentUtils.hpp"
    #include <libconfig.h++>
    #include <any>
    #include <cstddef>
    #include <cstdint>
    #include <iostream>
    #include <memory>
    #include <sys/types.h>
    #include <unordered_map>
    #include <vector>

namespace Components {

    /**
     * @enum MarioTypeID
     * 
     * @brief Enum to represent the type of the entity (e.g., ENEMY, ALLY).
     */
    enum class MarioTypeID : uint8_t {
        ENEMY = 0,
        PLAYER = 1,
        ITEM_BOX = 2,
        ALLY_PROJECTILE,
        ENEMY_PROJECTILE,
        BACKGROUND,
        UI,
        UNKNOWN = 255 // Optional: Handle an invalid state
    };

    /**
     * @class MarioType
     * 
     * @brief The MarioType class is a component that holds the MarioTypeID of an entity.
     */
    class MarioType : public AComponent<MarioType> {
    public:

        /**
         * @brief Constructor that takes an optional MarioTypeID or defaults to ENEMY.
         * 
         * @param id The type ID (as a uint8_t) of the entity.
         */
        MarioType(const MarioTypeID &id = MarioTypeID::ENEMY)
        :   AComponent("MarioType"),
            id(id)
        {
        }

        /**
         * @brief Constructor that creates a MarioType component from a configuration file.
         * 
         * @param config The libconfig::Setting object.
         */
        MarioType(const libconfig::Setting &config) : AComponent("MarioType")
        {
            int confId = 0;
            if (!config.lookupValue("id", confId)) {
                std::cerr << "Warning: 'id' not found in config. Using default value: 0 (ENEMY)\n";
                id = MarioTypeID::ENEMY;
            } else {
                id = static_cast<MarioTypeID>(confId);
            }
        }

        /**
         * @brief Get the size of the MarioType component (1 byte).
         * 
         * @return The size of the component.
         */
        std::size_t getSize() const override { return sizeof(uint8_t); }

        /**
         * @brief Serializes the MarioTypeID into a vector of bytes.
         * 
         * @return A vector containing the serialized data.
         */
        virtual std::vector<uint8_t> serialize() override {
            return {static_cast<uint8_t>(id)};
        }

        /**
         * @brief Deserializes the MarioTypeID from a vector of bytes.
         * 
         * @param data A vector containing the serialized MarioTypeID.
         */
        virtual void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != getSize()) {
                throw std::runtime_error("Invalid data size for MarioType component");
            }
            id = static_cast<MarioTypeID>(data[0]);
        }

        /**
         * @brief Adds the MarioType component to an entity using a vector of arguments.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine instance.
         * @param args A vector of arguments (expects the first argument to be a uint8_t).
         */
        virtual void addTo(
            ECS::Entity &to,
            Engine::GameEngine &engine,
            std::vector<std::any> args
        ) override {
            if (args.size() < 1) {
                throw std::runtime_error("Invalid number of arguments for MarioType component");
            }
            MarioTypeID typeId = std::any_cast<MarioTypeID>(args[0]);
            std::unique_ptr<Components::MarioType> typeComponent =
                engine.newComponent<Components::MarioType>(typeId);
            engine
                .getRegistry()
                .componentManager()
                .addComponent(to, std::move(typeComponent));
        }

        /**
         * @brief Adds the MarioType component to an entity using a configuration file.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine instance.
         * @param config A libconfig::Setting object for component configuration.
         */
        virtual void addTo(
            ECS::Entity &to,
            Engine::GameEngine &engine,
            libconfig::Setting &config
        ) override {
            int type = 0;
            if (!config.lookupValue("id", type)) {
                std::cerr << "Warning: 'id' not found in config. Using default value: 0 (ENEMY)\n";
                type = 0;
            }

            MarioTypeID typeId = static_cast<MarioTypeID>(type);

            attachAndUpdateComponent<Components::MarioType>(
                engine, to,
                typeId
            );
        }

        /**
         * @brief The MarioTypeID of the entity.
         */
        MarioTypeID id;
    };
}

#endif // MARIO_TYPE_HPP
