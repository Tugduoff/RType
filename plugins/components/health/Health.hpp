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

    class Health : public AComponent {
    public:

        Health() : currentHealth(100), maxHealth(100) {}

        Health(uint32_t current, uint32_t max) : currentHealth(current), maxHealth(max) {}

        std::vector<uint8_t> serialize() override {
            __network.currentHealth = htonl(currentHealth);
            __network.maxHealth = htonl(maxHealth);
            return std::vector<uint8_t>(__data, __data + sizeof(__data));
        };

        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != sizeof(__data))
                throw std::runtime_error("Invalid data size for Health component");

            currentHealth = ntohl(*reinterpret_cast<uint32_t *>(data.data()));
            maxHealth = ntohl(*reinterpret_cast<uint32_t *>(data.data() + 4));
        };

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
