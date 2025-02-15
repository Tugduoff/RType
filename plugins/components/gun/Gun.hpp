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
        static constexpr std::size_t MAX_ID_SIZE = 20; // Fixed size for network

        Gun(
            uint32_t bulletDamage = 10,
            uint32_t fireRate = 500,
            uint32_t bulletVelocityX = 8,
            uint32_t bulletVelocityY = 0,
            std::string spriteId = "shot1") :
            AComponent("Gun"),
            bulletDamage(bulletDamage),
            fireRate(fireRate),
            bulletVelocityX(bulletVelocityX),
            bulletVelocityY(bulletVelocityY),
            spriteId(spriteId),
            chrono() {}

        Gun(libconfig::Setting &config) : AComponent("Gun") {
            if (!config.lookupValue("bulletDamage", bulletDamage))
                bulletDamage = 10;
            if (!config.lookupValue("fireRate", fireRate))
                fireRate = 500;
            if (!config.lookupValue("bulletVelocityX", bulletVelocityX))
                bulletVelocityX = 8;
            if (!config.lookupValue("bulletVelocityY", bulletVelocityY))
                bulletVelocityY = 0;
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
            std::vector<uint8_t> data(getSize());

            // Serialize bulletDamage, fireRate, and bulletVelocityX/Y with endianness conversion
            uint32_t netBulletDamage = htonl(bulletDamage);
            uint32_t netFireRate = htonl(fireRate);
            uint32_t netBulletVelocityX = htonl(bulletVelocityX);
            uint32_t netBulletVelocityY = htonl(bulletVelocityY);

            // Copy the 4 fields into the data array
            std::memcpy(&data[0], &netBulletDamage, sizeof(uint32_t));
            std::memcpy(&data[4], &netFireRate, sizeof(uint32_t));
            std::memcpy(&data[8], &netBulletVelocityX, sizeof(uint32_t));
            std::memcpy(&data[12], &netBulletVelocityY, sizeof(uint32_t));

            std::string spriteIdLimited = std::string('\0', MAX_ID_SIZE);
            std::copy(spriteId.begin(), spriteId.end(), spriteIdLimited.begin());
            std::memcpy(&data[16], spriteIdLimited.c_str(), MAX_ID_SIZE);

            return data;
        }

        /**
         * @brief Deserializes the gun's data from the provided byte vector.
         * 
         * @param data A vector of bytes representing the serialized gun data.
         * @throws std::runtime_error If the data size is invalid.
         */
        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != getSize())
                throw std::runtime_error("Invalid data size for Gun component");

            // Deserialize bulletDamage, fireRate, and bulletVelocityX with endianness conversion
            std::memcpy(&bulletDamage, &data[0], sizeof(uint32_t));
            bulletDamage = ntohl(bulletDamage);

            std::memcpy(&fireRate, &data[4], sizeof(uint32_t));
            fireRate = ntohl(fireRate);

            std::memcpy(&bulletVelocityX, &data[8], sizeof(uint32_t));
            bulletVelocityX = ntohl(bulletVelocityX);

            std::memcpy(&bulletVelocityY, &data[12], sizeof(uint32_t));
            bulletVelocityY = ntohl(bulletVelocityY);

            // Deserialize the spriteId
            char spriteIdBuffer[MAX_ID_SIZE + 1];
            std::memcpy(spriteIdBuffer, &data[16], MAX_ID_SIZE);
            spriteIdBuffer[MAX_ID_SIZE] = '\0';

            spriteId = std::string(spriteIdBuffer);
        }

        /**
         * @brief Gets the size of the serialized data.
         * 
         * @return The size of the data, in bytes.
         */
        size_t getSize() const override {
            return MAX_ID_SIZE + 16; // 16 bytes for 4 uint32_t fields
        }

        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() != 5)
                throw std::runtime_error("Invalid number of arguments for Gun component");

            uint32_t bulletDamage = std::any_cast<uint32_t>(args[0]);
            uint32_t fireRate = std::any_cast<uint32_t>(args[1]);
            uint32_t bulletVelocityX = std::any_cast<uint32_t>(args[2]);
            uint32_t bulletVelocityY = std::any_cast<uint32_t>(args[3]);
            std::string spriteId = std::any_cast<std::string>(args[4]);

            engine.getRegistry().componentManager().addComponent<Components::Gun>(
                to, engine.newComponent<Components::Gun>(
                    bulletDamage,
                    fireRate,
                    bulletVelocityX,
                    bulletVelocityY,
                    spriteId));
        }

        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            int bulletDamage;
            int fireRate;
            int bulletVelocityX;
            int bulletVelocityY;
            std::string spriteId;

            if (!config.lookupValue("bulletDamage", bulletDamage))
                bulletDamage = 10;
            if (!config.lookupValue("fireRate", fireRate))
                fireRate = 500;
            if (!config.lookupValue("bulletVelocityX", bulletVelocityX))
                bulletVelocityX = 8;
            if (!config.lookupValue("bulletVelocityY", bulletVelocityY))
                bulletVelocityY =0;
            if (!config.lookupValue("spriteId", spriteId))
                spriteId = "shot1";

            attachAndUpdateComponent<Components::Gun>(
                engine, to,
                static_cast<uint32_t>(bulletDamage),
                static_cast<uint32_t>(fireRate),
                static_cast<uint32_t>(bulletVelocityX),
                static_cast<uint32_t>(bulletVelocityY),
                spriteId
            );
        }

        uint32_t bulletDamage;
        uint32_t fireRate;
        uint32_t bulletVelocityX;
        uint32_t bulletVelocityY;
        std::string spriteId;
        Chrono chrono;

    };
}

#endif // GUN_HPP
