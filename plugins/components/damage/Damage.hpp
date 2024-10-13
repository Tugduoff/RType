/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Damage.hpp file
*/

#ifndef DAMAGE_HPP
    #define DAMAGE_HPP

    #include "plugins/components/AComponent.hpp"
    #include "GameEngine/GameEngine.hpp"

    #ifdef _WIN32
        #include <windows.h>
        #pragma comment(lib, "ws2_32.lib")
    #else
        #include <arpa/inet.h>
    #endif

    #include <vector>
    #include <stdexcept>
    #include <libconfig.h++>

/**
 * @brief Damage component class for ECS.
 * 
 * This class represents a damage component for an entity, storing a damage value.
 */
namespace Components {

    /**
     * @brief A component representing damage.
     * 
     * This class stores the damage value and provides methods for serialization and
     * deserialization, allowing it to be used in network transmissions.
     */
    class Damage : public AComponent<Damage> {
    public:
        /**
         * @brief Default constructor for the Damage component.
         * 
         * Initializes the damage to a default value.
         */
        Damage(uint32_t damage = 10) : AComponent("Damage"), damage(damage) {}

        /**
         * @brief Constructor that initializes the Damage component from a configuration.
         * 
         * @param config The configuration setting to extract the component data from.
         */
        Damage(libconfig::Setting &config) : AComponent("Damage") {
            if (!config.lookupValue("damage", damage))
                damage = 10;
        }

        /**
         * @brief Default destructor for the Damage component.
         */
        ~Damage() = default;

        /**
         * @brief Serializes the damage's data into a byte vector.
         * 
         * @return A vector of bytes representing the serialized damage data.
         */
        std::vector<uint8_t> serialize() override {
            __network.damage = htonl(*reinterpret_cast<uint32_t*>(&damage));
            return std::vector<uint8_t>(__data, __data + sizeof(__data));
        }

        /**
         * @brief Deserializes the damage's data from the provided byte vector.
         * 
         * @param data A vector of bytes representing the serialized damage data.
         * @throws std::runtime_error If the data size is invalid.
         */
        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != sizeof(__data))
                throw std::runtime_error("Invalid data size for Damage component");
            damage = ntohl(*reinterpret_cast<uint32_t *>(data.data()));
        }

        /**
         * @brief Gets the size of the serialized data.
         * 
         * @return The size of the data, in bytes.
         */
        size_t getSize() const override {
            return sizeof(__data);
        }

        /**
         * @brief Adds the Damage component to an entity.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param args The arguments to pass to the component constructor.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() != 1)
                throw std::runtime_error("Invalid number of arguments for Damage component");
            uint32_t damage = std::any_cast<uint32_t>(args[0]);
            engine.getRegistry().componentManager().addComponent<Components::Damage>(to, engine.newComponent<Components::Damage>(damage));
        }

        /**
         * @brief Adds the Damage component to an entity from a configuration setting.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param config The configuration setting to extract the component data from.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            int damage;

            if (!config.lookupValue("damage", damage))
                damage = 10;
            std::unique_ptr<Components::Damage> dmg = engine.newComponent<Components::Damage>(static_cast<uint32_t>(damage));
            engine.getRegistry().componentManager().addComponent<Components::Damage>(to, std::move(dmg));
        }

        uint32_t damage;

    private:
        union {
            struct {
                uint32_t damage;
            } __network;
            uint8_t __data[4];
        };
    };
};

#endif // DAMAGE_HPP
