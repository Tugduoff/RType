/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Health.hpp file
*/

#ifndef HEALTH_HPP
    #define HEALTH_HPP

    #include "plugins/components/AComponent.hpp"
    #include <vector>
    #include <stdexcept>
    #include <arpa/inet.h>

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
