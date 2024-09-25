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

/**
 * @brief Position component class for ECS.
 * 
 * This class represents the position of an entity, storing the x and y coordinates,
 * as well as a layer to manage the entity's depth in the rendering order. It also provides
 * methods to serialize and deserialize the position data.
 */
class Position : public AComponent {
public:
    /**
     * @brief Default constructor for the Position component.
     * 
     * Initializes the position components (x, y, layer) to zero.
     */
    Position() : x(0), y(0), layer(0) {};

    /**
     * @brief Parameterized constructor for the Position component.
     * 
     * Initializes the position with the provided x and y coordinates and defaults the layer to zero.
     * 
     * @param x The X coordinate of the position.
     * @param y The Y coordinate of the position.
     */
    Position(uint32_t x, uint32_t y) : x(x), y(y), layer(0) {};

    /**
     * @brief Default destructor for the Position component.
     */
    ~Position() = default;

    /**
     * @brief Serializes the position components into a byte vector.
     * 
     * Converts the x, y, and layer components into network byte order for transmission or storage.
     * 
     * @return A vector of bytes representing the serialized position components.
     */
    std::vector<uint8_t> serialize() override {
        __network.x = htonl(x);
        __network.y = htonl(y);
        __network.layer = htonl(layer);
        return std::vector<uint8_t>(__data, __data + sizeof(__data));
    };

    /**
     * @brief Deserializes the position components from the provided byte vector.
     * 
     * Reads the x, y, and layer components in network byte order.
     * 
     * @param data A vector of bytes representing the serialized position components.
     * @throws std::runtime_error If the data size is invalid.
     */
    void deserialize(std::vector<uint8_t> &data) override {
        if (data.size() != sizeof(__data))
            throw std::runtime_error("Invalid data size for position component");
        x = ntohl(*reinterpret_cast<uint32_t *>(data.data()));
        y = ntohl(*reinterpret_cast<uint32_t *>(data.data() + 4));
        layer = ntohl(*reinterpret_cast<uint32_t *>(data.data() + 8));
    };

    /**
     * @brief Gets the size of the serialized data.
     * 
     * @return The size of the data, in bytes.
     */
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
