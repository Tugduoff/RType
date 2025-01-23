/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Position3d.hpp file
*/

#ifndef POSITION_3D_HPP
    #define POSITION_3D_HPP

    #include "plugins/components/AComponent.hpp"
    #include "GameEngine/GameEngine.hpp"
    #include "components/AComponent.hpp"
    #include "utils/ComponentUtils.hpp"
     #ifdef _WIN32
        #include <windows.h>
        #pragma comment(lib, "ws2_32.lib")
    #else
        #include <arpa/inet.h>
    #endif
    #include <vector>
    #include <stdexcept>
    #include <libconfig.h++>
    #include <iostream>
    #include <iomanip>

namespace Components {
    /**
     * @brief Position3d component class in Components.
     * 
     * This class represents the position of an entity, storing the x and y coordinates,
     * as well as a z to manage the entity's depth in the rendering order. It also provides
     * methods to serialize and deserialize the position data.
     */
    class Position3d : public AComponent<Position3d> {
    public:
        /**
         * @brief Default constructor for the Position3d component.
         * 
         * Initializes the position components (x, y, z) to zero.
         */
        Position3d() : AComponent("Position3d"), x(0), y(0), z(0), floatX(0), floatY(0), floatZ(0) {};
        Position3d(libconfig::Setting &config) : AComponent("Position3d")
        {
            if (!config.lookupValue("x", x)) x = 0;
            if (!config.lookupValue("y", y)) y = 0;
            if (!config.lookupValue("z", z)) z = 1;
            floatX = x;
            floatY = y;
            floatZ = z;

            std::cerr << "######### POSITION: " << std::setw(5) << x << " " << std::setw(5) << y << " " << std::setw(5) << z << std::endl;
        }

        /**
         * @brief Parameterized constructor for the Position3d component.
         * 
         * Initializes the position with the provided x and y coordinates and defaults the z to zero.
         * 
         * @param x The X coordinate of the position.
         * @param y The Y coordinate of the position.
         */
        Position3d(int32_t x, int32_t y, int32_t z) : AComponent("Position3d"), x(x), y(y), z(z), floatX(x), floatY(y), floatZ(z) {};

        /**
         * @brief Default destructor for the Position3d component.
         */
        ~Position3d() = default;

        /**
         * @brief Serializes the position components into a byte vector.
         * 
         * Converts the x, y, and z components into network byte order for transmission or storage.
         * 
         * @return A vector of bytes representing the serialized position components.
         */
        std::vector<uint8_t> serialize() override {
            __network.x = htonl(x);
            __network.y = htonl(y);
            __network.z = htonl(z);
            return std::vector<uint8_t>(__data, __data + sizeof(__data));
        };

        /**
         * @brief Deserializes the position components from the provided byte vector.
         * 
         * Reads the x, y, and z components in network byte order.
         * 
         * @param data A vector of bytes representing the serialized position components.
         * @throws std::runtime_error If the data size is invalid.
         */
        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != sizeof(__data))
                throw std::runtime_error("Invalid data size for position component");
            x = ntohl(*reinterpret_cast<int32_t *>(data.data()));
            y = ntohl(*reinterpret_cast<int32_t *>(data.data() + 4));
            z = ntohl(*reinterpret_cast<int32_t *>(data.data() + 8));
            floatX = x;
            floatY = y;
            floatZ = z;

            std::cerr << "######### POSITION: " << std::setw(5) << x << " " << std::setw(5) << y << " " << std::setw(5) << z << std::endl;
        };

        /**
         * @brief Gets the size of the serialized data.
         * 
         * @return The size of the data, in bytes.
         */
        size_t getSize() const override { return sizeof(__data); };

        /**
         * @brief Adds the Position3d component to an entity.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param args The arguments to pass to the component constructor.
         * 
         * @note This function will add the Position3d component to the entity.
         * @note The arguments should be a tuple of three int32_t values: x, y, and z.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() != 3)
                throw std::runtime_error("Invalid number of arguments for Position3d component");
            int32_t x = std::any_cast<int32_t>(args[0]);
            int32_t y = std::any_cast<int32_t>(args[1]);
            int32_t z = std::any_cast<int32_t>(args[2]);
            engine.getRegistry().componentManager().addComponent<Components::Position3d>(to, engine.newComponent<Components::Position3d>(x, y, z));
        };

        /**
         * @brief Adds the Position3d component to an entity from a configuration setting.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param config The configuration setting to extract the component data from.
         * 
         * @note This function will add the Position3d component to the entity.
         * @note The configuration setting should contain the keys 'x', 'y', and 'z'.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            int xVal = 0, yVal = 0, zVal = 1;

            if (!config.lookupValue("x", xVal)) {
                std::cerr << "Warning: 'x' not found in config. Using default value: 0\n";
                xVal = 0;
            }

            if (!config.lookupValue("y", yVal)) {
                std::cerr << "Warning: 'y' not found in config. Using default value: 0\n";
                yVal = 0;
            }

            if (!config.lookupValue("z", zVal)) {
                std::cerr << "Warning: 'z' not found in config. Using default value: 1\n";
                zVal = 1;
            }

            std::cout << "x: " << xVal << " y: " << yVal << " z: " << zVal << std::endl;
            std::cout << std::endl;

            attachAndUpdateComponent<Components::Position3d>(
                engine, to,
                static_cast<int32_t>(xVal),
                static_cast<int32_t>(yVal),
                static_cast<int32_t>(zVal)
            );
        };

        int32_t x;
        int32_t y;
        int32_t z;
        float floatX;
        float floatY;
        float floatZ;

    private:
        union {
            struct {
                int32_t x;
                int32_t y;
                int32_t z;
            } __network;
            uint8_t __data[12];
        };
    };
};

#endif // POSITION_3D_HPP
