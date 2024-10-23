/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Collider.hpp file
*/

#ifndef COLLIDER_HPP
    #define COLLIDER_HPP

    #include "plugins/components/AComponent.hpp"
    #include "GameEngine/GameEngine.hpp"
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
     * @brief Collider component class in Components.
     * 
     * This class represents a rectangle collider for an entity, storing the width and height of the collider.
     * It provides methods to serialize and deserialize the collider data for network transmission or storage.
     */
    class Collider : public AComponent<Collider> {
    public:
        /**
         * @brief Default constructor
         * 
         * Initializes the rectangle collider with default width and height of 0.
         */
        Collider(uint32_t width = 1, uint32_t height = 1) : AComponent(std::string("Collider")), width(width), height(height) {};

        /**
         * @brief Constructor using configuration.
         * 
         * Initializes the rectangle collider based on config settings, with default values
         * if lookupValue fails.
         */
        Collider(libconfig::Setting &config) : AComponent(std::string("Collider")) {
            if (!config.lookupValue("width", width)) {
                width = 1;
            }
            if (!config.lookupValue("height", height)) {
                height = 1;
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
                throw std::runtime_error("Invalid data size for Collider component");

            width = ntohl(*reinterpret_cast<uint32_t *>(data.data()));
            height = ntohl(*reinterpret_cast<uint32_t *>(data.data() + 4));
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
         * @brief Adds the Collider component to an entity.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param args The arguments to pass to the component constructor.
         * 
         * @note The arguments should be a tuple containing the width and height.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() != 2)
                throw std::runtime_error("Invalid number of arguments for Collider component");

            uint32_t width = std::any_cast<uint32_t>(args[0]);
            uint32_t height = std::any_cast<uint32_t>(args[1]);

            auto collider = engine.newComponent<Components::Collider>(width, height);
            engine.getRegistry().componentManager().addComponent<Components::Collider>(to, std::move(collider));
        };

        /**
         * @brief Adds the Collider component to an entity from a configuration setting.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param config The configuration setting to extract the component data from.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            int width = 0, height = 0; 

            if (!config.lookupValue("width", width)) {
                std::cerr << "Warning: 'width' not found in config. Using default value: 1\n";
                width = 1;
            }

            if (!config.lookupValue("height", height)) {
                std::cerr << "Warning: 'height' not found in config. Using default value: 1\n";
                height = 1;
            }

            std::cout << "width: " << width << " height: " << height << std::endl;

            std::unique_ptr<Components::Collider> collider = engine.newComponent<Components::Collider>(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
            Components::IComponent *rawComponent = collider.get();
            engine
                .getRegistry()
                .componentManager()
                .addComponent<Components::Collider>(to, std::move(collider));
            std::cerr << "Collider component added to entity: " << to << std::endl;
            engine.updateComponent(to, rawComponent->getId(), rawComponent->serialize());
            std::cerr << "Collider component updated" << std::endl;
            std::cout << std::endl;
        }

        uint32_t width;
        uint32_t height;

    private:
        union {
            struct {
                uint32_t width;
                uint32_t height;
            } __network;
            uint8_t __data[8];
        };
    };
};

#endif // COLLIDER_HPP
