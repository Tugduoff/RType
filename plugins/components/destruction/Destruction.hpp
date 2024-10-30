/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Destruction.hpp file
*/

#ifndef DESTRUCTION_HPP
    #define DESTRUCTION_HPP

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
     * @brief Destruction component class in Components.
     * 
     * This class stores all the elements needed for the effect that happens after the entity is killed
     */
    class Destruction : public AComponent<Destruction> {
    public:
        static constexpr std::size_t MAX_ID_SIZE = 20; // Fixed size for network
        static constexpr std::size_t MAX_SOUND_SIZE = 40; // Fixed size for network

        /**
         * @brief Default constructor
         * 
         * Initializes the Destruction with default width and height of 1.
         */
        Destruction(uint32_t duration = 1000, std::string id = "enemy", std::tuple<std::string, uint8_t, uint8_t, uint8_t> sound = {
            std::make_tuple("AMBIENT", 100, 1, 0)
        }, std::pair<uint32_t, uint32_t> scale = {
            std::make_pair(100, 100)
        })
            : AComponent(std::string("Destruction")), duration(duration), id(id), sound(sound), scale(scale) {}

        /**
         * @brief Constructor using configuration.
         * 
         * Initializes the Destruction based on config settings, with default values
         */
        Destruction(libconfig::Setting &config) : AComponent(std::string("Destruction")) {
            int durationVal = 1000;
            std::string idVal = "enemy";
            std::string soundPathVal = "";
            int soundVolumeVal = 100;
            int soundPitchVal = 1;
            bool soundLoopVal = false;
            int scaleWidthVal = 100;
            int scaleHeightVal = 100;

            config.lookupValue("duration", durationVal);
            config.lookupValue("id", idVal);

            const libconfig::Setting &soundSetting = config.lookup("sound");

            soundSetting.lookupValue("path", soundPathVal);
            soundSetting.lookupValue("volume", soundVolumeVal);
            soundSetting.lookupValue("pitch", soundPitchVal);
            soundSetting.lookupValue("loop", soundLoopVal);

            const libconfig::Setting &scaleSetting = config.lookup("scale");

            scaleSetting.lookupValue("width", scaleWidthVal);
            scaleSetting.lookupValue("height", scaleHeightVal);

            duration = durationVal;
            id = idVal;
            sound = std::make_tuple(soundPathVal, soundVolumeVal, soundPitchVal, soundLoopVal);
            scale = std::make_pair(scaleWidthVal, scaleHeightVal);
        }

        /**
         * @brief Serialize the Destruction data
         * 
         * Serializes the width and height into a byte vector for transmission or storage.
         * 
         * @return std::vector<uint8_t> Serialized data.
         */
        std::vector<uint8_t> serialize() override {
            std::vector<uint8_t> data(getSize());
            uint32_t networkDuration = htonl(this->duration);

            std::copy(reinterpret_cast<uint8_t *>(&networkDuration), reinterpret_cast<uint8_t *>(&networkDuration) + sizeof(networkDuration), data.begin());

            std::string newId;
            newId.resize(MAX_ID_SIZE);
            std::copy(id.begin(), id.end(), newId.begin());

            std::string soundPath = std::get<0>(sound);
            std::string newSound;
            newSound.resize(MAX_SOUND_SIZE);
            std::copy(soundPath.begin(), soundPath.end(), newSound.begin());

            std::copy(newId.begin(), newId.end(), data.begin() + sizeof(networkDuration));
            std::copy(newSound.begin(), newSound.end(), data.begin() + sizeof(networkDuration) + MAX_ID_SIZE);

            size_t offset = sizeof(networkDuration) + MAX_ID_SIZE + MAX_SOUND_SIZE;

            data[offset] = std::get<1>(sound);
            data[offset + 1] = std::get<2>(sound);
            data[offset + 2] = std::get<3>(sound);

            uint32_t networkScaleWidth = htonl(this->scale.first);
            uint32_t networkScaleHeight = htonl(this->scale.second);

            std::copy(reinterpret_cast<uint8_t *>(&networkScaleWidth),
                reinterpret_cast<uint8_t *>(&networkScaleWidth) + sizeof(networkScaleWidth),
                data.begin() + offset + 3);
            std::copy(reinterpret_cast<uint8_t *>(&networkScaleHeight),
                reinterpret_cast<uint8_t *>(&networkScaleHeight) + sizeof(networkScaleHeight),
                data.begin() + offset + sizeof(networkScaleWidth) + 3);

            std::cerr << "Serialized Destruction:" << std::endl;
            for (auto &byte : data) {
                std::cerr << (unsigned)byte << " ";
            }
            std::cerr << std::endl;
            return data;
        }

        /**
         * @brief Deserialize the Destruction data
         * 
         * Deserializes the width and height from the provided byte vector.
         * 
         * @param data The byte vector containing serialized data.
         * @throws std::runtime_error If the data size is invalid.
         */
        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != getSize())
                throw std::runtime_error("Invalid data size for Destruction component");

            std::cerr << "Deserialized Destruction:" << std::endl;
            for (auto &byte : data) {
                std::cerr << (unsigned)byte << " ";
            }
            std::cerr << std::endl;

            uint32_t networkDuration = *reinterpret_cast<uint32_t *>(data.data());
            duration = ntohl(networkDuration);

            size_t offset = sizeof(networkDuration) + MAX_ID_SIZE + MAX_SOUND_SIZE;

            id = std::string(data.begin() + sizeof(networkDuration), data.begin() + sizeof(networkDuration) + MAX_ID_SIZE);
            id.erase(std::find(id.begin(), id.end(), '\0'), id.end());

            std::string soundPath = std::string(data.begin() + sizeof(networkDuration) + MAX_ID_SIZE, data.begin() + offset);
            soundPath.erase(std::find(soundPath.begin(), soundPath.end(), '\0'), soundPath.end());

            uint8_t volume = data[offset];
            uint8_t pitch = data[offset + 1];
            uint8_t loop = data[offset + 2];

            uint32_t networkScaleWidth = *reinterpret_cast<uint32_t *>(data.data() + offset + 3);
            uint32_t networkScaleHeight = *reinterpret_cast<uint32_t *>(data.data() + offset + sizeof(networkScaleWidth) + 3);

            sound = std::make_tuple(soundPath, volume, pitch, loop);
            scale = std::make_pair(ntohl(networkScaleWidth), ntohl(networkScaleHeight));

            std::cerr << "Deserialized Destruction: duration=" << duration << " id=" << id << " sound=" << soundPath << " volume=" << (unsigned)volume << " pitch=" << (unsigned)pitch << " loop=" << (unsigned)loop << " scaleWidth=" << scale.first << " scaleHeight=" << scale.second << std::endl;
        }

        /**
         * @brief Get the fixed size of the serialized data
         * 
         * @return size_t Size in bytes
         */
        size_t getSize() const override { return 15 + MAX_ID_SIZE + MAX_SOUND_SIZE; }

        /**
         * @brief Adds the Destruction component to an entity.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param args The arguments to pass to the component constructor.
         * 
         * @note The arguments should be two uint32_t containing the width and height.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() != 4)
                throw std::runtime_error("Invalid number of arguments for Destruction component");

            uint32_t duration = std::any_cast<uint32_t>(args[0]);
            std::string id = std::any_cast<std::string>(args[1]);
            std::tuple<std::string, uint8_t, uint8_t, uint8_t> sound = std::any_cast<std::tuple<std::string, uint8_t, uint8_t, uint8_t>>(args[2]);
            std::pair<uint32_t, uint32_t> scale = std::any_cast<std::pair<uint32_t, uint32_t>>(args[3]);

            auto Destruction = engine.newComponent<Components::Destruction>(duration, id, sound, scale);
            engine.getRegistry().componentManager().addComponent<Components::Destruction>(to, std::move(Destruction));
        };

        /**
         * @brief Adds the Destruction component to an entity from a configuration setting.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param config The configuration setting to extract the component data from.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            int durationVal = 1000;
            std::string idVal = "enemy";
            std::string soundPathVal = "";
            int soundVolumeVal = 100;
            int soundPitchVal = 1;
            bool soundLoopVal = false;
            int scaleWidthVal = 100;
            int scaleHeightVal = 100;

            config.lookupValue("duration", durationVal);
            config.lookupValue("id", idVal);

            const libconfig::Setting &sound = config.lookup("sound");

            sound.lookupValue("path", soundPathVal);
            sound.lookupValue("volume", soundVolumeVal);
            sound.lookupValue("pitch", soundPitchVal);
            sound.lookupValue("loop", soundLoopVal);

            const libconfig::Setting &scale = config.lookup("scale");

            scale.lookupValue("width", scaleWidthVal);
            scale.lookupValue("height", scaleHeightVal);

            attachAndUpdateComponent<Components::Destruction>(
                engine, to,
                durationVal, idVal,
                std::make_tuple(soundPathVal, soundVolumeVal, soundPitchVal, soundLoopVal),
                std::make_pair(scaleWidthVal, scaleHeightVal)
            );
        }

        uint32_t duration;
        std::string id;
        // sound path, sound volume, sound pitch, sound loop
        std::tuple<std::string, uint8_t, uint8_t, bool> sound;
        // width, height
        std::pair<uint32_t, uint32_t> scale;

    };
};

#endif // DESTRUCTION_HPP
