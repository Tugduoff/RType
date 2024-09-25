/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Velocity.hpp file
*/

#ifndef Velocity_HPP
    #define Velocity_HPP

    #include "AComponent.hpp"
    #include <arpa/inet.h>
    #include <vector>
    #include <stdexcept>

class Velocity : public AComponent {
    public:
        Velocity() : x(0), y(0) {};
        Velocity(uint32_t x, uint32_t y) : x(x), y(y) {};
        ~Velocity() = default;

        std::vector<uint8_t> serialize() override {
            __network.x = htonl(x);
            __network.y = htonl(y);
            return std::vector<uint8_t>(__data, __data + sizeof(__data));
        };

        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != sizeof(__data))
                throw std::runtime_error("Invalid data size for position component");
            x = ntohl(*reinterpret_cast<uint32_t *>(data.data()));
            y = ntohl(*reinterpret_cast<uint32_t *>(data.data() + 4));
        };

        size_t getSize() const override { return sizeof(__data); };

        uint32_t x;
        uint32_t y;
    private:
        union {
            struct {
                uint32_t x;
                uint32_t y;
            } __network;
            uint8_t __data[8];
        };
};

#endif // VELOCITY_HPP