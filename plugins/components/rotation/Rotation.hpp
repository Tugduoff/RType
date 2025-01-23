/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Rotation.hpp file
*/

#ifndef ROTATION_HPP
    #define ROTATION_HPP

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

namespace Components {
    /**
     * @brief Rotation component class in Components.
     * 
     * This class represents the Rotation of an entity, storing the x and y coordinates,
     * as well as a z to manage the entity's depth in the rendering order. It also provides
     * methods to serialize and deserialize the Rotation data.
     */
    class Rotation : public AComponent<Rotation> {
    public:
        /**
         * @brief Default constructor for the Rotation component.
         * 
         * Initializes the Rotation components (x, y, z) to zero.
         */
        Rotation() : AComponent("Rotation"), x(0), y(0), z(0) {};
        Rotation(libconfig::Setting &config) : AComponent("Rotation")
        {
            if (!config.lookupValue("x", x)) x = 0;
            if (!config.lookupValue("y", y)) y = 0;
            if (!config.lookupValue("z", z)) z = 1;
        }

        /**
         * @brief Parameterized constructor for the Rotation component.
         * 
         * Initializes the Rotation with the provided x and y coordinates and defaults the z to zero.
         * 
         * @param x The X coordinate of the Rotation.
         * @param y The Y coordinate of the Rotation.
         * @param z The Z coordinate of the Rotation.
         */
        Rotation(int32_t x, int32_t y, int32_t z) : AComponent("Rotation"), x(x), y(y), z(z) {};

        /**
         * @brief Default destructor for the Rotation component.
         */
        ~Rotation() = default;

        /**
         * @brief Serializes the Rotation components into a byte vector.
         * 
         * Converts the x, y, and z components into network byte order for transmission or storage.
         * 
         * @return A vector of bytes representing the serialized Rotation components.
         */
        std::vector<uint8_t> serialize() override {
            __network.x = htonl(x);
            __network.y = htonl(y);
            __network.z = htonl(z);
            return std::vector<uint8_t>(__data, __data + sizeof(__data));
        };

        /**
         * @brief Deserializes the Rotation components from the provided byte vector.
         * 
         * Reads the x, y, and z components in network byte order.
         * 
         * @param data A vector of bytes representing the serialized Rotation components.
         * @throws std::runtime_error If the data size is invalid.
         */
        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != sizeof(__data))
                throw std::runtime_error("Invalid data size for Rotation component");
            x = ntohl(*reinterpret_cast<int32_t *>(data.data()));
            y = ntohl(*reinterpret_cast<int32_t *>(data.data() + 4));
            z = ntohl(*reinterpret_cast<int32_t *>(data.data() + 8));
        };

        /**
         * @brief Gets the size of the serialized data.
         * 
         * @return The size of the data, in bytes.
         */
        size_t getSize() const override { return sizeof(__data); };

        /**
         * @brief Adds the Rotation component to an entity.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param args The arguments to pass to the component constructor.
         * 
         * @note This function will add the Rotation component to the entity.
         * @note The arguments should be three int32_t values: x, y, and z.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() != 3)
                throw std::runtime_error("Invalid number of arguments for Rotation component");
            int32_t x = std::any_cast<int32_t>(args[0]);
            int32_t y = std::any_cast<int32_t>(args[1]);
            int32_t z = std::any_cast<int32_t>(args[2]);
            engine.getRegistry().componentManager().addComponent<Components::Rotation>(to, engine.newComponent<Components::Rotation>(x, y, z));
        };

        /**
         * @brief Adds the Rotation component to an entity from a configuration setting.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param config The configuration setting to extract the component data from.
         * 
         * @note This function will add the Rotation component to the entity.
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

            attachAndUpdateComponent<Components::Rotation>(
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

#endif // ROTATION_HPP
