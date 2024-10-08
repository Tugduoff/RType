/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Health.hpp file
*/

#ifndef HEALTH_HPP
    #define HEALTH_HPP

    #include "plugins/components/AComponent.hpp"
    #include "GameEngine/GameEngine.hpp"
    #include <vector>
    #include <stdexcept>
    #include <arpa/inet.h>
    #include <libconfig.h++>
    #include <any>

namespace Components {

    /**
     * @brief Health component for ECS
     * 
     * This class represents the health of an entity, storing both current and maximum health values.
     * It provides methods for serialization and deserialization to allow for network transmission or storage.
     */
    class Health : public AComponent {
    public:

        /**
         * @brief Default constructor for Health component.
         * 
         * Initializes current and max health to default values of 100.
         */
        Health() : currentHealth(100), maxHealth(100) {}
        Health(libconfig::Setting &config);

        /**
         * @brief Parameterized constructor for Health component.
         * 
         * Initializes the current and max health with given values.
         * 
         * @param current The initial value of current health.
         * @param max The initial value of maximum health.
         */
        Health(uint32_t current, uint32_t max) : currentHealth(current), maxHealth(max) {}

        /**
         * @brief Serializes the health values into a byte vector.
         * 
         * Converts current and max health into network byte order for transmission or storage.
         * 
         * @return A vector of bytes representing the serialized health component.
         */
        std::vector<uint8_t> serialize() override {
            __network.currentHealth = htonl(currentHealth);
            __network.maxHealth = htonl(maxHealth);
            return std::vector<uint8_t>(__data, __data + sizeof(__data));
        };

        /**
         * @brief Deserializes the health component from a byte vector.
         * 
         * Reads the current and max health values from network byte order.
         * 
         * @param data A vector of bytes representing the serialized health component.
         * @throws std::runtime_error If the data size is invalid.
         */
        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != sizeof(__data))
                throw std::runtime_error("Invalid data size for Health component");

            currentHealth = ntohl(*reinterpret_cast<uint32_t *>(data.data()));
            maxHealth = ntohl(*reinterpret_cast<uint32_t *>(data.data() + 4));
        };

        /**
         * @brief Get the size of the serialized data.
         * 
         * @return The size of the data, in bytes.
         */
        size_t getSize() const override {
            return sizeof(__data);
        };

        /**
         * @brief Adds the Health component to an entity.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param args The arguments to pass to the component constructor.
         * 
         * @note This function will add the Health component to the entity.
         * @note The arguments should be a tuple of two uint32_t values: currentHealth and maxHealth.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() != 2)
                throw std::runtime_error("Invalid number of arguments for Health component");
            
            uint32_t currentHealth = std::any_cast<uint32_t>(args[0]);
            uint32_t maxHealth = std::any_cast<uint32_t>(args[1]);

            engine.getRegistry().componentManager().addComponent<Components::Health>(to, engine.newComponent<Components::Health>(currentHealth, maxHealth));
        };

        /**
         * @brief Adds the Health component to an entity from a configuration setting.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param config The configuration setting to extract the component data from.
         * 
         * @note This function will add the Health component to the entity.
         * @note The configuration setting should contain the keys 'currentHealth' and 'maxHealth'.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            int currentVal = 0, maxVal = 0;

            if (
                !config.lookupValue("currentHealth", currentVal) ||
                !config.lookupValue("maxHealth", maxVal)) {
                throw std::invalid_argument("Failed to retrieve values for 'currentHealth' or 'maxHealth'");
            }

            std::unique_ptr<Components::Health> health = engine.newComponent<Components::Health>(static_cast<uint32_t>(currentVal), static_cast<uint32_t>(maxVal));
            engine.getRegistry().componentManager().addComponent<Components::Health>(to, std::move(health));
        };

        uint32_t currentHealth;
        uint32_t maxHealth;

    private:
        union {
            struct {
                uint32_t currentHealth;
                uint32_t maxHealth;
            } __network;
            uint8_t __data[8];
        };
    };
};

#endif // HEALTH_HPP
