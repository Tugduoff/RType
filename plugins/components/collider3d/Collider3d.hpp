/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Collider3d.hpp file
*/

#ifndef COLLIDER_3D_HPP
    #define COLLIDER_3D_HPP

    #include "plugins/components/AComponent.hpp"
    #include "GameEngine/GameEngine.hpp"
    #include "utils/ComponentUtils.hpp"
    #ifdef _WIN32
        #include <windows.h>
        #pragma comment(lib, "ws2_32.lib")
    #else
        #include <arpa/inet.h>
    #endif
    #include <vector>
    #include <stdexcept>
    #include <cstdint>
    #include <libconfig.h++>

namespace Components {
    /**
     * @brief Collider3d component class in Components.
     * 
     * This class represents a rectangle collider for an entity, storing the width and height of the collider.
     * It provides methods to serialize and deserialize the collider data for network transmission or storage.
     */
    class Collider3d : public AComponent<Collider3d> {
    public:
        /**
         * @brief Default constructor
         * 
         * Initializes the rectangle collider with default width and height of 0.
         */
        Collider3d(uint32_t width = 1, uint32_t height = 1, uint32_t depth = 1) : AComponent(std::string("Collider3d")), width(width), height(height), depth(depth) {};

        /**
         * @brief Constructor using configuration.
         * 
         * Initializes the rectangle collider based on config settings, with default values
         * if lookupValue fails.
         */
        Collider3d(libconfig::Setting &config) : AComponent(std::string("Collider3d")) {
            if (!config.lookupValue("width", width)) {
                width = 1;
            }
            if (!config.lookupValue("height", height)) {
                height = 1;
            }
            if (!config.lookupValue("depth", depth)) {
                depth = 1;
            }
        }        

        /**
         * @brief Serialize the collider data
         * 
         * Serializes the width and height into a byte vector for transmission or storage.
         * 
         * @return std::vector<uint8_t> Serialized data.
         */
        std::vector<uint8_t> serialize() override {
            __network.width = htonl(width);
            __network.height = htonl(height);
            __network.depth = htonl(depth);
            return std::vector<uint8_t>(__data, __data + sizeof(__data));
        }

        /**
         * @brief Deserialize the collider data
         * 
         * Deserializes the width and height from the provided byte vector.
         * 
         * @param data The byte vector containing serialized data.
         * @throws std::runtime_error If the data size is invalid.
         */
        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != getSize())
                throw std::runtime_error("Invalid data size for Collider3d component");

            width = ntohl(*reinterpret_cast<uint32_t *>(data.data()));
            height = ntohl(*reinterpret_cast<uint32_t *>(data.data() + 4));
            depth = ntohl(*reinterpret_cast<uint32_t *>(data.data() + 8));
        }

        /**
         * @brief Get the fixed size of the serialized data
         * 
         * @return size_t Size in bytes
         */
        size_t getSize() const override {
            return sizeof(__data);
        }

        /**
         * @brief Adds the Collider3d component to an entity.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param args The arguments to pass to the component constructor.
         * 
         * @note The arguments should be a tuple containing the width and height.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() != 3)
                throw std::runtime_error("Invalid number of arguments for Collider3d component");

            uint32_t width = std::any_cast<uint32_t>(args[0]);
            uint32_t height = std::any_cast<uint32_t>(args[1]);
            uint32_t depth = std::any_cast<uint32_t>(args[2]);

            auto collider = engine.newComponent<Components::Collider3d>(width, height, depth);
            engine.getRegistry().componentManager().addComponent<Components::Collider3d>(to, std::move(collider));
        };

        /**
         * @brief Adds the Collider3d component to an entity from a configuration setting.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param config The configuration setting to extract the component data from.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            int width = 0, height = 0, depth = 0; 

            if (!config.lookupValue("width", width)) {
                std::cerr << "Warning: 'width' not found in config. Using default value: 1\n";
                width = 1;
            }

            if (!config.lookupValue("height", height)) {
                std::cerr << "Warning: 'height' not found in config. Using default value: 1\n";
                height = 1;
            }

            if (!config.lookupValue("depth", depth)) {
                std::cerr << "Warning: 'depth' not found in config. Using default value: 1\n";
                depth = 1;
            }

            attachAndUpdateComponent<Components::Collider3d>(
                engine, to,
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height),
                static_cast<uint32_t>(depth)
            );
        }

        uint32_t width;
        uint32_t height;
        uint32_t depth;

    private:
        union {
            struct {
                uint32_t width;
                uint32_t height;
                uint32_t depth;
            } __network;
            uint8_t __data[12];
        };
    };
};

#endif // COLLIDER_3D_HPP
