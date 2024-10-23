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
    #include "utils/Chrono.hpp"

    #ifdef _WIN32
        #include <windows.h>
        #pragma comment(lib, "ws2_32.lib")
    #else
        #include <arpa/inet.h>
    #endif

    #include <vector>
    #include <stdexcept>
    #include <libconfig.h++>
    #include <string>
    #include <cstring>

/**
 * @brief Gun component class for ECS.
 * 
 * This class represents a gun component for an entity, storing bulletDamage, fire rate,
 * and bullet velocity values. It also provides methods to serialize and deserialize the gun data.
 */
namespace Components {

    class Gun : public AComponent<Gun> {
    public:
        Gun(
            uint32_t bulletDamage = 10,
            uint32_t fireRate = 500,
            uint32_t bulletVelocity = 8,
            std::string spriteId = "shot1") :
            AComponent("Gun"),
            bulletDamage(bulletDamage),
            fireRate(fireRate),
            bulletVelocity(bulletVelocity),
            spriteId(spriteId),
            chrono() {}

        Gun(libconfig::Setting &config) : AComponent("Gun") {
            if (!config.lookupValue("bulletDamage", bulletDamage))
                bulletDamage = 10;
            if (!config.lookupValue("fireRate", fireRate))
                fireRate = 500;
            if (!config.lookupValue("bulletVelocity", bulletVelocity))
                bulletVelocity = 8;
            if (!config.lookupValue("spriteId", spriteId))
                spriteId = "shot1";
            chrono.restart();
        }

        ~Gun() = default;

        /**
         * @brief Serializes the gun's data into a byte vector.
         * 
         * @return A vector of bytes representing the serialized gun data.
         */
        std::vector<uint8_t> serialize() override {
            std::vector<uint8_t> data(16);

            // Serialize bulletDamage, fireRate, and bulletVelocity with endianness conversion
            uint32_t netBulletDamage = htonl(bulletDamage);
            uint32_t netFireRate = htonl(fireRate);
            uint32_t netBulletVelocity = htonl(bulletVelocity);

            // Copy the 3 fields into the data array
            std::memcpy(&data[0], &netBulletDamage, sizeof(uint32_t));
            std::memcpy(&data[4], &netFireRate, sizeof(uint32_t));
            std::memcpy(&data[8], &netBulletVelocity, sizeof(uint32_t));

            // Limit spriteId to a maximum of 4 characters to fit within remaining space
            std::string spriteIdLimited = spriteId.substr(0, 4);
            std::memcpy(&data[12], spriteIdLimited.c_str(), spriteIdLimited.size());

            return data;
        }

        /**
         * @brief Deserializes the gun's data from the provided byte vector.
         * 
         * @param data A vector of bytes representing the serialized gun data.
         * @throws std::runtime_error If the data size is invalid.
         */
        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != 16)
                throw std::runtime_error("Invalid data size for Gun component");

            // Deserialize bulletDamage, fireRate, and bulletVelocity with endianness conversion
            std::memcpy(&bulletDamage, &data[0], sizeof(uint32_t));
            bulletDamage = ntohl(bulletDamage);

            std::memcpy(&fireRate, &data[4], sizeof(uint32_t));
            fireRate = ntohl(fireRate);

            std::memcpy(&bulletVelocity, &data[8], sizeof(uint32_t));
            bulletVelocity = ntohl(bulletVelocity);

            // Deserialize the spriteId (limited to 4 characters)
            spriteId = std::string(data.begin() + 12, data.end());
        }

        /**
         * @brief Gets the size of the serialized data.
         * 
         * @return The size of the data, in bytes.
         */
        size_t getSize() const override {
            return 16; // 12 bytes for 3 uint32_t fields and 4 bytes for spriteId
        }

        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() != 4)
                throw std::runtime_error("Invalid number of arguments for Gun component");

            uint32_t bulletDamage = std::any_cast<uint32_t>(args[0]);
            uint32_t fireRate = std::any_cast<uint32_t>(args[1]);
            uint32_t bulletVelocity = std::any_cast<uint32_t>(args[2]);
            std::string spriteId = std::any_cast<std::string>(args[3]);

            engine.getRegistry().componentManager().addComponent<Components::Gun>(
                to, engine.newComponent<Components::Gun>(bulletDamage, fireRate, bulletVelocity, spriteId));
        }

        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            int bulletDamage;
            int fireRate;
            int bulletVelocity;
            std::string spriteId;

            if (!config.lookupValue("bulletDamage", bulletDamage))
                bulletDamage = 10;
            if (!config.lookupValue("fireRate", fireRate))
                fireRate = 500;
            if (!config.lookupValue("bulletVelocity", bulletVelocity))
                bulletVelocity = 8;
            if (!config.lookupValue("spriteId", spriteId))
                spriteId = "shot1";

            std::unique_ptr<Components::Gun> gun =
                engine.newComponent<Components::Gun>(static_cast<uint32_t>(bulletDamage), static_cast<uint32_t>(fireRate),
                                                     static_cast<uint32_t>(bulletVelocity), spriteId);
            Components::IComponent *rawComponent = gun.get();
            engine
                .getRegistry()
                .componentManager()
                .addComponent<Components::Gun>(to, std::move(gun));
            std::cerr << "Gun component added to entity: " << to << std::endl;
            engine.updateComponent(to, rawComponent->getId(), rawComponent->serialize());
            std::cerr << "Gun component updated" << std::endl;
        }

        uint32_t bulletDamage;
        uint32_t fireRate;
        uint32_t bulletVelocity;
        std::string spriteId;
        Chrono chrono;
    };
}

#endif // GUN_HPP
