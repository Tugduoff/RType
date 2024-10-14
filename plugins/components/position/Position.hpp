/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Position.hpp file
*/

#ifndef POSITION_HPP
    #define POSITION_HPP

    #include "plugins/components/AComponent.hpp"
    #include "GameEngine/GameEngine.hpp"
    #include "components/AComponent.hpp"
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
     * @brief Position component class in Components.
     * 
     * This class represents the position of an entity, storing the x and y coordinates,
     * as well as a layer to manage the entity's depth in the rendering order. It also provides
     * methods to serialize and deserialize the position data.
     */
    class Position : public AComponent<Position> {
    public:
        /**
         * @brief Default constructor for the Position component.
         * 
         * Initializes the position components (x, y, layer) to zero.
         */
        Position() : AComponent("Position"), x(0), y(0), layer(0) {};
        Position(libconfig::Setting &config) : AComponent("Position")
        {
            if (!config.lookupValue("x", x)) x = 0;
            if (!config.lookupValue("y", y)) y = 0;
            if (!config.lookupValue("layer", layer)) layer = 1;
        }

        /**
         * @brief Parameterized constructor for the Position component.
         * 
         * Initializes the position with the provided x and y coordinates and defaults the layer to zero.
         * 
         * @param x The X coordinate of the position.
         * @param y The Y coordinate of the position.
         */
        Position(uint32_t x, uint32_t y, uint32_t layer) : AComponent("Position"), x(x), y(y), layer(layer) {};

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

        /**
         * @brief Adds the Position component to an entity.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param args The arguments to pass to the component constructor.
         * 
         * @note This function will add the Position component to the entity.
         * @note The arguments should be a tuple of three uint32_t values: x, y, and layer.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() != 3)
                throw std::runtime_error("Invalid number of arguments for Position component");
            uint32_t x = std::any_cast<uint32_t>(args[0]);
            uint32_t y = std::any_cast<uint32_t>(args[1]);
            uint32_t layer = std::any_cast<uint32_t>(args[2]);
            engine.getRegistry().componentManager().addComponent<Components::Position>(to, engine.newComponent<Components::Position>(x, y, layer));
        };

        /**
         * @brief Adds the Position component to an entity from a configuration setting.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param config The configuration setting to extract the component data from.
         * 
         * @note This function will add the Position component to the entity.
         * @note The configuration setting should contain the keys 'x', 'y', and 'layer'.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            int xVal = 0, yVal = 0, layerVal = 1;

            if (!config.lookupValue("x", xVal)) {
                std::cerr << "Warning: 'x' not found in config. Using default value: 0\n";
                xVal = 0;
            }

            if (!config.lookupValue("y", yVal)) {
                std::cerr << "Warning: 'y' not found in config. Using default value: 0\n";
                yVal = 0;
            }

            if (!config.lookupValue("layer", layerVal)) {
                std::cerr << "Warning: 'layer' not found in config. Using default value: 1\n";
                layerVal = 1;
            }

            std::cout << "x: " << xVal << " y: " << yVal << " layer: " << layerVal << std::endl;

            std::unique_ptr<Components::Position> pos = engine.newComponent<Components::Position>(static_cast<uint32_t>(xVal), static_cast<uint32_t>(yVal), static_cast<uint32_t>(layerVal));
            engine.getRegistry().componentManager().addComponent<Components::Position>(to, std::move(pos));
            std::cout << std::endl;
        };

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
};

#endif // POSITION_HPP
