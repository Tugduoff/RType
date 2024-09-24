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
    #include <vector>
    #include <stdexcept>

class Position : public AComponent {
    public:
        Position() : x(0), y(0), layer(0) {};
        Position(uint32_t x, uint32_t y) : x(x), y(y), layer(layer) {};
        ~Position() = default;

        std::vector<uint8_t> serialize() override {
            __network.x = htonl(x);
            __network.y = htonl(y);
            __network.layer = htonl(layer);
            return std::vector<uint8_t>(__data, __data + sizeof(__data));
        };

        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != sizeof(__data))
                throw std::runtime_error("Invalid data size for position component");
            x = ntohl(*reinterpret_cast<uint32_t *>(data.data()));
            y = ntohl(*reinterpret_cast<uint32_t *>(data.data() + 4));
            layer = ntohl(*reinterpret_cast<uint32_t *>(data.data() + 8));
        };

        size_t getSize() const override { return sizeof(__data); };

        uint32_t x;
        uint32_t y;
        uint32_t layer;
    private:
        union {
            struct {
                uint32_t x;
                uint32_t y;
                uint32_t layer;
            } __network;
            uint8_t __data[12];
        };
};

#endif // POSITION_HPP
