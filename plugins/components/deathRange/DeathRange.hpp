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
    #include "Chrono.hpp"

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
 * values. It also provides methods to serialize and deserialize the DeathRange data.
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
         * Initializes the maximum and minimum values to default values.
         */
        DeathRange(uint32_t maximum = 2000, uint32_t minimum = 0) : AComponent("DeathRange"), maximum(maximum), minimum(minimum) {}

        /**
         * @brief Constructor that initializes the DeathRange component from a configuration.
         * 
         * @param config The configuration setting to extract the component data from.
         */
        DeathRange(libconfig::Setting &config) : AComponent("DeathRange") {
            if (!config.lookupValue("maximum", maximum))
                maximum = 2000;
            if (!config.lookupValue("minimum", minimum))
                minimum = 0;
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
            __network.maximum = htonl(*reinterpret_cast<uint32_t*>(&maximum));
            __network.minimum = htonl(*reinterpret_cast<uint32_t*>(&minimum));
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
            maximum = ntohl(*reinterpret_cast<uint32_t *>(data.data()));
            minimum = ntohl(*reinterpret_cast<uint32_t *>(data.data() + 4));
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
            if (args.size() != 2)
                throw std::runtime_error("Invalid number of arguments for DeathRange component");
            uint32_t maximum = std::any_cast<uint32_t>(args[0]);
            uint32_t minimum = std::any_cast<uint32_t>(args[1]);
            engine.getRegistry().componentManager().addComponent<Components::DeathRange>(to, engine.newComponent<Components::DeathRange>(maximum, minimum));
        }

        /**
         * @brief Adds the DeathRange component to an entity from a configuration setting.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param config The configuration setting to extract the component data from.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            int maximum;
            int minimum;

            if (!config.lookupValue("max", maximum))
                maximum = 2000;
            if (!config.lookupValue("min", minimum))
                minimum = 0;
            std::unique_ptr<Components::DeathRange> DeathRange = engine.newComponent<Components::DeathRange>(static_cast<uint32_t>(maximum), static_cast<uint32_t>(minimum));
            engine.getRegistry().componentManager().addComponent<Components::DeathRange>(to, std::move(DeathRange));
        }

        uint32_t maximum;
        uint32_t minimum;

    private:
        union {
            struct {
                uint32_t maximum;
                uint32_t minimum;
            } __network;
            uint8_t __data[8];
        };
    };
};

#endif // DeathRange_HPP
