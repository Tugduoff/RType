/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Velocity.hpp file
*/

#ifndef VELOCITY_HPP
    #define VELOCITY_HPP

    #include "plugins/components/AComponent.hpp"
    #include "GameEngine.hpp"
    #include <arpa/inet.h>
    #include <vector>
    #include <stdexcept>

namespace Components {
    /**
     * @brief Velocity component class for ECS.
     * 
     * This class represents the velocity of an entity, storing the x and y components,
     * and provides methods to serialize and deserialize the data.
     */
    class Velocity : public AComponent {
    public:
        /**
         * @brief Default constructor for the Velocity component.
         * 
         * Initializes the velocity components (x and y) to zero.
         */
        Velocity() : x(0), y(0) {};
        Velocity(libconfig::Setting &config);

        /**
         * @brief Parameterized constructor for the Velocity component.
         * 
         * Initializes the velocity with the provided x and y components.
         * 
         * @param x The X component of the velocity.
         * @param y The Y component of the velocity.
         */
        Velocity(uint32_t x, uint32_t y) : x(x), y(y) {};

        /**
         * @brief Default destructor for the Velocity component.
         */
        ~Velocity() = default;

        /**
         * @brief Serializes the velocity components into a byte vector.
         * 
         * Converts the x and y velocity components into network byte order for transmission or storage.
         * 
         * @return A vector of bytes representing the serialized velocity components.
         */
        std::vector<uint8_t> serialize() override {
            __network.x = htonl(x);
            __network.y = htonl(y);
            return std::vector<uint8_t>(__data, __data + sizeof(__data));
        };

        /**
         * @brief Deserializes the velocity components from the provided byte vector.
         * 
         * Reads the x and y velocity components in network byte order.
         * 
         * @param data A vector of bytes representing the serialized velocity components.
         * @throws std::runtime_error If the data size is invalid.
         */
        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != sizeof(__data))
                throw std::runtime_error("Invalid data size for position component");
            x = ntohl(*reinterpret_cast<uint32_t *>(data.data()));
            y = ntohl(*reinterpret_cast<uint32_t *>(data.data() + 4));
        };

        /**
         * @brief Gets the size of the serialized data.
         * 
         * @return The size of the data, in bytes.
         */
        size_t getSize() const override {
            return sizeof(__data);
        };

        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() != 2)
                throw std::runtime_error("Invalid number of arguments for Velocity component");
            uint32_t x = std::any_cast<uint32_t>(args[0]);
            uint32_t y = std::any_cast<uint32_t>(args[1]);
            engine.getRegistry().componentManager().addComponent<Components::Velocity>(to, engine.newComponent<Components::Velocity>(x, y));
        };

        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            int xVal = 0, yVal = 0;

            if (
                !config.lookupValue("x", xVal) ||
                !config.lookupValue("y", yVal)) {
                throw std::invalid_argument("Failed to retrieve values for 'x', 'y'");
            }

            std::cout << "x: " << xVal << " y: " << yVal << std::endl;

            std::unique_ptr<Components::Velocity> pos = engine.newComponent<Components::Velocity>(static_cast<uint32_t>(xVal), static_cast<uint32_t>(yVal));
            engine.getRegistry().componentManager().addComponent<Components::Velocity>(to, std::move(pos));
            std::cout << std::endl;
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
};

#endif // VELOCITY_HPP
