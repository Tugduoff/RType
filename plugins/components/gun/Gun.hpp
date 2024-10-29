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
        static constexpr std::size_t MAX_SOUND_SIZE = 40; // Fixed size for network
        static constexpr std::size_t MAX_ID_SIZE = 20; // Fixed size for network

        Gun(
            uint32_t bulletDamage = 10,
            uint32_t fireRate = 500,
            uint32_t bulletVelocity = 8,
            std::string spriteId = "shot1",
            std::string soundPath = "",
            uint8_t volume = 100,
            uint8_t pitch = 1) :
            AComponent("Gun"),
            bulletDamage(bulletDamage),
            fireRate(fireRate),
            bulletVelocity(bulletVelocity),
            spriteId(spriteId),
            soundPath(soundPath),
            volume(volume),
            pitch(pitch),
            chrono() {}

        Gun(libconfig::Setting &config) : AComponent("Gun") {
            int volumeVal = 100;
            int pitchVal = 1;

            if (!config.lookupValue("bulletDamage", bulletDamage))
                bulletDamage = 10;
            if (!config.lookupValue("fireRate", fireRate))
                fireRate = 500;
            if (!config.lookupValue("bulletVelocity", bulletVelocity))
                bulletVelocity = 8;
            if (!config.lookupValue("spriteId", spriteId))
                spriteId = "shot1";
            if (!config.lookupValue("soundPath", soundPath))
                soundPath = "";
            if (!config.lookupValue("soundVolume", volumeVal))
                volume = 100;
            if (!config.lookupValue("soundPitch", pitchVal))
                pitch = 1;

            volume = static_cast<uint8_t>(volumeVal);
            pitch = static_cast<uint8_t>(pitchVal);
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

            // Serialize bulletDamage, fireRate, and bulletVelocity with endianness conversion
            uint32_t netBulletDamage = htonl(bulletDamage);
            uint32_t netFireRate = htonl(fireRate);
            uint32_t netBulletVelocity = htonl(bulletVelocity);

            // Copy the 3 fields into the data array
            std::memcpy(&data[0], &netBulletDamage, sizeof(uint32_t));
            std::memcpy(&data[4], &netFireRate, sizeof(uint32_t));
            std::memcpy(&data[8], &netBulletVelocity, sizeof(uint32_t));

            std::string spriteIdLimited = std::string('\0', MAX_ID_SIZE);
            std::copy(spriteId.begin(), spriteId.end(), spriteIdLimited.begin());
            std::memcpy(&data[12], spriteIdLimited.c_str(), MAX_ID_SIZE);

            std::string soundPathLimited = std::string('\0', MAX_SOUND_SIZE);
            std::copy(soundPath.begin(), soundPath.end(), soundPathLimited.begin());
            std::memcpy(&data[12 + MAX_ID_SIZE], soundPathLimited.c_str(), MAX_SOUND_SIZE);

            // Add the volume and pitch
            data[12 + MAX_ID_SIZE + MAX_SOUND_SIZE] = volume;
            data[12 + MAX_ID_SIZE + MAX_SOUND_SIZE + 1] = pitch;

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

            // Deserialize bulletDamage, fireRate, and bulletVelocity with endianness conversion
            std::memcpy(&bulletDamage, &data[0], sizeof(uint32_t));
            bulletDamage = ntohl(bulletDamage);

            std::memcpy(&fireRate, &data[4], sizeof(uint32_t));
            fireRate = ntohl(fireRate);

            std::memcpy(&bulletVelocity, &data[8], sizeof(uint32_t));
            bulletVelocity = ntohl(bulletVelocity);

            // Deserialize the spriteId
            char spriteIdBuffer[MAX_ID_SIZE + 1];
            std::memcpy(spriteIdBuffer, &data[12], MAX_ID_SIZE);
            spriteIdBuffer[MAX_ID_SIZE] = '\0';

            spriteId = std::string(spriteIdBuffer);

            // Deserialize the soundPath
            char soundPathBuffer[MAX_SOUND_SIZE + 1];
            std::memcpy(soundPathBuffer, &data[12 + MAX_ID_SIZE], MAX_SOUND_SIZE);
            soundPathBuffer[MAX_SOUND_SIZE] = '\0';

            soundPath = std::string(soundPathBuffer);

            // Deserialize the volume and pitch
            volume = data[12 + MAX_ID_SIZE + MAX_SOUND_SIZE];
            pitch = data[12 + MAX_ID_SIZE + MAX_SOUND_SIZE + 1];
        }

        /**
         * @brief Gets the size of the serialized data.
         * 
         * @return The size of the data, in bytes.
         */
        size_t getSize() const override {
            return MAX_ID_SIZE + MAX_SOUND_SIZE + 12 + 2; // 12 bytes for 3 uint32_t fields + 2 bytes for 2 uint8_t fields
        }

        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() != 7)
                throw std::runtime_error("Invalid number of arguments for Gun component");

            uint32_t bulletDamage = std::any_cast<uint32_t>(args[0]);
            uint32_t fireRate = std::any_cast<uint32_t>(args[1]);
            uint32_t bulletVelocity = std::any_cast<uint32_t>(args[2]);
            std::string spriteId = std::any_cast<std::string>(args[3]);
            std::string soundPath = std::any_cast<std::string>(args[4]);
            uint8_t volume = std::any_cast<uint8_t>(args[5]);
            uint8_t pitch = std::any_cast<uint8_t>(args[6]);

            engine.getRegistry().componentManager().addComponent<Components::Gun>(
                to, engine.newComponent<Components::Gun>(
                    bulletDamage,
                    fireRate,
                    bulletVelocity,
                    spriteId,
                    soundPath,
                    volume,
                    pitch));
        }

        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            int bulletDamage;
            int fireRate;
            int bulletVelocity;
            int volumeVal;
            int pitchVal;
            std::string soundPath;
            std::string spriteId;

            if (!config.lookupValue("bulletDamage", bulletDamage))
                bulletDamage = 10;
            if (!config.lookupValue("fireRate", fireRate))
                fireRate = 500;
            if (!config.lookupValue("bulletVelocity", bulletVelocity))
                bulletVelocity = 8;
            if (!config.lookupValue("spriteId", spriteId))
                spriteId = "shot1";
            if (!config.lookupValue("soundPath", soundPath))
                soundPath = "";
            if (!config.lookupValue("soundVolume", volumeVal))
                volume = 100;
            if (!config.lookupValue("soundPitch", pitchVal))
                pitch = 1;

            attachAndUpdateComponent<Components::Gun>(
                engine, to,
                static_cast<uint32_t>(bulletDamage),
                static_cast<uint32_t>(fireRate),
                static_cast<uint32_t>(bulletVelocity),
                spriteId,
                soundPath,
                static_cast<uint8_t>(volumeVal),
                static_cast<uint8_t>(pitchVal)
            );
        }

        uint32_t bulletDamage;
        uint32_t fireRate;
        uint32_t bulletVelocity;
        std::string spriteId;
        std::string soundPath;
        uint8_t volume;
        uint8_t pitch;
        Chrono chrono;

    };
}

#endif // GUN_HPP
