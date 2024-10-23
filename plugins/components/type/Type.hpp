/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Type.hpp file
*/

#ifndef TYPE_HPP
    #define TYPE_HPP

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
     * @enum TypeID
     * 
     * @brief Enum to represent the type of the entity (e.g., ENEMY, ALLY).
     */
    enum class TypeID : uint8_t {
        ENEMY = 0,
        ALLY,
        ALLY_PROJECTILE,
        ENEMY_PROJECTILE,
        BACKGROUND,
        UI,
        UNKNOWN = 255 // Optional: Handle an invalid state
    };

    /**
     * @class Type
     * 
     * @brief The Type class is a component that holds the TypeID of an entity.
     */
    class Type : public AComponent<Type> {
    public:

        /**
         * @brief Constructor that takes an optional TypeID or defaults to ENEMY.
         * 
         * @param id The type ID (as a uint8_t) of the entity.
         */
        Type(const TypeID &id = TypeID::ENEMY)
        :   AComponent("Type"),
            id(id)
        {
        }

        /**
         * @brief Constructor that creates a Type component from a configuration file.
         * 
         * @param config The libconfig::Setting object.
         */
        Type(const libconfig::Setting &config) : AComponent("Type")
        {
            int confId = 0;
            if (!config.lookupValue("id", confId)) {
                std::cerr << "Warning: 'id' not found in config. Using default value: 0 (ENEMY)\n";
                id = TypeID::ENEMY;
            } else {
                id = static_cast<TypeID>(confId);
            }
        }

        /**
         * @brief Get the size of the Type component (1 byte).
         * 
         * @return The size of the component.
         */
        std::size_t getSize() const override { return sizeof(uint8_t); }

        /**
         * @brief Serializes the TypeID into a vector of bytes.
         * 
         * @return A vector containing the serialized data.
         */
        virtual std::vector<uint8_t> serialize() override {
            return {static_cast<uint8_t>(id)};
        }

        /**
         * @brief Deserializes the TypeID from a vector of bytes.
         * 
         * @param data A vector containing the serialized TypeID.
         */
        virtual void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != getSize()) {
                throw std::runtime_error("Invalid data size for Type component");
            }
            id = static_cast<TypeID>(data[0]);
        }

        /**
         * @brief Adds the Type component to an entity using a vector of arguments.
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
                throw std::runtime_error("Invalid number of arguments for Type component");
            }
            TypeID typeId = std::any_cast<TypeID>(args[0]);
            std::unique_ptr<Components::Type> typeComponent =
                engine.newComponent<Components::Type>(typeId);
            engine
                .getRegistry()
                .componentManager()
                .addComponent(to, std::move(typeComponent));
        }

        /**
         * @brief Adds the Type component to an entity using a configuration file.
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

            TypeID typeId = static_cast<TypeID>(type);

            attachAndUpdateComponent<Components::Type>(
                engine, to,
                typeId
            );
        }

        /**
         * @brief The TypeID of the entity.
         */
        TypeID id;
    };
}

#endif // TYPE_HPP
