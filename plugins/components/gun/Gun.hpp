/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Gun.hpp file
*/

#ifndef GUN_HPP
#define GUN_HPP

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

/**
 * @brief Gun component class for ECS.
 * 
 * This class represents a gun component for an entity, storing damage and fire rate
 * values. It also provides methods to serialize and deserialize the gun data.
 */
namespace Components {

    class Gun : public AComponent<Gun> {
    public:
        /**
         * @brief Default constructor for the Gun component.
         * 
         * Initializes the damage and fire rate to default values.
         */
        Gun(uint32_t damage = 10, uint32_t fireRate = 500) : AComponent("Gun") {}

        /**
         * @brief Constructor that initializes the Gun component from a configuration.
         * 
         * @param config The configuration setting to extract the component data from.
         */
        Gun(libconfig::Setting &config) : AComponent("Gun") {
            if (!config.lookupValue("damage", damage))
                damage = 10;
            if (!config.lookupValue("fireRate", fireRate))
                fireRate = 500;
        }

        /**
         * @brief Default destructor for the Gun component.
         */
        ~Gun() = default;

        /**
         * @brief Serializes the gun's data into a byte vector.
         * 
         * @return A vector of bytes representing the serialized gun data.
         */
        std::vector<uint8_t> serialize() override {
            __network.damage = htonl(*reinterpret_cast<uint32_t*>(&damage));
            __network.fireRate = htonl(*reinterpret_cast<uint32_t*>(&fireRate));
            return std::vector<uint8_t>(__data, __data + sizeof(__data));
        }

        /**
         * @brief Deserializes the gun's data from the provided byte vector.
         * 
         * @param data A vector of bytes representing the serialized gun data.
         * @throws std::runtime_error If the data size is invalid.
         */
        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != sizeof(__data))
                throw std::runtime_error("Invalid data size for Gun component");
            damage = ntohl(*reinterpret_cast<uint32_t *>(data.data()));
            fireRate = ntohl(*reinterpret_cast<uint32_t *>(data.data() + 4));
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
         * @brief Adds the Gun component to an entity.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param args The arguments to pass to the component constructor.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() != 2)
                throw std::runtime_error("Invalid number of arguments for Gun component");
            uint32_t damage = std::any_cast<uint32_t>(args[0]);
            uint32_t fireRate = std::any_cast<uint32_t>(args[1]);
            engine.getRegistry().componentManager().addComponent<Components::Gun>(to, engine.newComponent<Components::Gun>(damage, fireRate));
        }

        /**
         * @brief Adds the Gun component to an entity from a configuration setting.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param config The configuration setting to extract the component data from.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            if (!config.lookupValue("damage", damage) || !config.lookupValue("fireRate", fireRate)) {
                damage = 10;
                fireRate = 500;
                std::cout << "Using default values: damage = " << damage << ", fireRate = " << fireRate << std::endl;
            }
            std::unique_ptr<Components::Gun> gun = engine.newComponent<Components::Gun>(damage, fireRate);
            engine.getRegistry().componentManager().addComponent<Components::Gun>(to, std::move(gun));
        }

        uint32_t damage;
        uint32_t fireRate;

    private:
        union {
            struct {
                uint32_t damage;
                uint32_t fireRate;
            } __network;
            uint8_t __data[8];
        };
    };
};

#endif // GUN_HPP
