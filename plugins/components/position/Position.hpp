/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Position.hpp file
*/

#ifndef POSITION_HPP
    #define POSITION_HPP

    #include "AComponent.hpp"
    #include <arpa/inet.h>

class Position : public AComponent<8> {
    public:
        Position() : x(0), y(0) {};
        Position(uint32_t x, uint32_t y) : x(x), y(y) {};
        ~Position() = default;

        std::span<uint8_t, 8> serialize() override {
            __network.x = htonl(x);
            __network.y = htonl(y);
            return std::span<uint8_t, 8>(__data);
        };
        void deserialize(std::span<uint8_t, 8> &data) override {
            x = ntohl(*reinterpret_cast<uint32_t *>(data.data()));
            y = ntohl(*reinterpret_cast<uint32_t *>(data.data() + 4));
        };

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

#endif // POSITION_HPP
