/*
** EPITECH PROJECT, 2024
** RType
** File description:
** DeathRange.hpp file
*/

#ifndef DeathRange_HPP
    #define DeathRange_HPP

    #include "plugins/components/AComponent.hpp"
    #include "GameEngine/GameEngine.hpp"
    #include "components/AComponent.hpp"
    #include "utils/Chrono.hpp"
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

/**
 * @brief DeathRange component class for ECS.
 * 
 * This class represents a DeathRange component for an entity, storing maximum and minimum
 * values on the X and Y axis. It also provides methods to serialize and deserialize the DeathRange data.
 */
namespace Components {
    /**
     * @brief A component representing a DeathRange with minimum and maximum attributes.
     * 
     * This class stores the DeathRange's maximum and minimum, allowing it to be 
     * serialized and deserialized for network transmission. It also provides 
     * methods for initializing the component from configuration settings.
     */
    class DeathRange : public AComponent<DeathRange> {
    public:
        /**
         * @brief Default constructor for the DeathRange component.
         * 
         * Initializes the DeathRange component with default values for the maximum and minimum
         * in both axis.
         */
        DeathRange(
            uint32_t maxX = 1920,
            uint32_t maxY = 1080,
            uint32_t minX = 0,
            uint32_t minY = 0) :
            AComponent("DeathRange"), maxX(maxX), maxY(maxY), minX(minX), minY(minY) {}

        /**
         * @brief Constructor that initializes the DeathRange component from a configuration.
         * 
         * @param config The configuration setting to extract the component data from.
         */
        DeathRange(libconfig::Setting &config) : AComponent("DeathRange") {
            if (!config.lookupValue("maxX", maxX))
                maxX = 1920;
            if (!config.lookupValue("maxY", maxY))
                maxY = 1080;
            if (!config.lookupValue("minX", minX))
                minX = 0;
            if (!config.lookupValue("minY", minY))
                minY = 0;
        }

        /**
         * @brief Default destructor for the DeathRange component.
         */
        ~DeathRange() = default;

        /**
         * @brief Serializes the DeathRange's data into a byte vector.
         * 
         * @return A vector of bytes representing the serialized DeathRange data.
         */
        std::vector<uint8_t> serialize() override {
            __network.maxX = htonl(*reinterpret_cast<uint32_t*>(&maxX));
            __network.maxY = htonl(*reinterpret_cast<uint32_t*>(&maxY));
            __network.minX = htonl(*reinterpret_cast<uint32_t*>(&minX));
            __network.minY = htonl(*reinterpret_cast<uint32_t*>(&minY));
            return std::vector<uint8_t>(__data, __data + sizeof(__data));
        }

        /**
         * @brief Deserializes the DeathRange's data from the provided byte vector.
         * 
         * @param data A vector of bytes representing the serialized DeathRange data.
         * @throws std::runtime_error If the data size is invalid.
         */
        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != sizeof(__data))
                throw std::runtime_error("Invalid data size for DeathRange component");
            maxX = ntohl(*reinterpret_cast<uint32_t *>(data.data()));
            maxY = ntohl(*reinterpret_cast<uint32_t *>(data.data() + 4));
            minX = ntohl(*reinterpret_cast<uint32_t *>(data.data() + 8));
            minY = ntohl(*reinterpret_cast<uint32_t *>(data.data() + 12));
        }

        /**
         * @brief Gets the size of the serialized data.
         * 
         * @return The size of the data, in bytes.
         */
        size_t getSize() const override {
            return sizeof(__data);
        }

        /**
         * @brief Adds the DeathRange component to an entity.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param args The arguments to pass to the component constructor.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() != 4)
                throw std::runtime_error("Invalid number of arguments for DeathRange component");
            uint32_t maxX = std::any_cast<uint32_t>(args[0]);
            uint32_t maxY = std::any_cast<uint32_t>(args[1]);
            uint32_t minX = std::any_cast<uint32_t>(args[2]);
            uint32_t minY = std::any_cast<uint32_t>(args[3]);
            engine.getRegistry().componentManager().addComponent<Components::DeathRange>(to, engine.newComponent<Components::DeathRange>(maxX, maxY, minX, minY));
        }

        /**
         * @brief Adds the DeathRange component to an entity from a configuration setting.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param config The configuration setting to extract the component data from.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            int maxX, maxY, minX, minY;

            if (!config.lookupValue("maxX", maxX))
                maxX = 1920;
            if (!config.lookupValue("maxY", maxY))
                maxY = 1080;
            if (!config.lookupValue("minX", minX))
                minX = 0;
            if (!config.lookupValue("minY", minY))
                minY = 0;

            attachAndUpdateComponent<Components::DeathRange>(
                engine, to,
                static_cast<uint32_t>(maxX),
                static_cast<uint32_t>(maxY),
                static_cast<uint32_t>(minX),
                static_cast<uint32_t>(minY)
            );
        }

        uint32_t maxX;
        uint32_t maxY;
        uint32_t minX;
        uint32_t minY;

    private:
        union {
            struct {
                uint32_t maxX;
                uint32_t maxY;
                uint32_t minX;
                uint32_t minY;
            } __network;
            uint8_t __data[16];
        };
    };
};

#endif // DeathRange_HPP
