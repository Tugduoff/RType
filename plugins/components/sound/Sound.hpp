/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Sound.hpp file
*/

#ifndef SOUND_HPP
    #define SOUND_HPP

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
     * @brief Sound component class in Components.
     * 
     * This class represents the Sound of an entity, storing the width and height of the entity.
     */
    class Sound : public AComponent<Sound> {
    public:
        static constexpr std::size_t MAX_SOUND_SIZE = 40; // Fixed size for network

        /**
         * @brief Default constructor
         * 
         * Initializes the Sound with default width and height of 1.
         */
        Sound(std::string sound = "", uint8_t volume = 100, uint8_t pitch = 1, uint8_t loop = 0)
            : AComponent(std::string("Sound")), sound(sound), volume(volume), pitch(pitch), loop(loop) {
                std::cerr << "Sound created with path: " << sound << " volume: " << (unsigned)volume << " pitch: " << (unsigned)pitch << " loop: " << (unsigned)loop << std::endl;
                if (this->sound.size() > MAX_SOUND_SIZE) {
                    this->sound.resize(MAX_SOUND_SIZE);  // Truncate if longer than 40
                }
            };

        /**
         * @brief Constructor using configuration.
         * 
         * Initializes the Sound based on config settings, with default values
         */
        Sound(libconfig::Setting &config) : AComponent(std::string("Sound")) {
            int volumeVal = 100;
            int pitchVal = 1;
            bool loop = false;

            if (!config.lookupValue("path", sound)) {
                sound = "";
            }
            if (!config.lookupValue("volume", volumeVal)) {
                volumeVal = 100;
            }
            if (!config.lookupValue("pitch", pitchVal)) {
                pitchVal = 100;
            }
            if (!config.lookupValue("loop", loop)) {
                loop = false;
            }
            if (this->sound.size() > MAX_SOUND_SIZE) {
                this->sound.resize(MAX_SOUND_SIZE);  // Truncate if longer than 40
            }
        }

        /**
         * @brief Serialize the Sound data
         * 
         * Serializes the width and height into a byte vector for transmission or storage.
         * 
         * @return std::vector<uint8_t> Serialized data.
         */
        std::vector<uint8_t> serialize() override {
            std::cerr << "Serializing Sound: " << sound << std::endl;

            std::vector<uint8_t> data(MAX_SOUND_SIZE + sizeof(volume) + sizeof(pitch) + sizeof(loop), 0);  // Initialize with zeros (40 bytes + 3 bytes)
            std::copy(sound.begin(), sound.end(), data.begin());  // Copy the string into the vector
            std::copy(reinterpret_cast<uint8_t *>(&volume), reinterpret_cast<uint8_t *>(&volume) + sizeof(volume), data.begin() + MAX_SOUND_SIZE);
            std::copy(reinterpret_cast<uint8_t *>(&pitch), reinterpret_cast<uint8_t *>(&pitch) + sizeof(pitch), data.begin() + MAX_SOUND_SIZE + sizeof(volume));
            std::copy(reinterpret_cast<uint8_t *>(&loop), reinterpret_cast<uint8_t *>(&loop) + sizeof(loop), data.begin() + MAX_SOUND_SIZE + sizeof(volume) + sizeof(pitch));

            for (auto &byte : data) {
                std::cerr << (unsigned)byte << " ";
            }
            return data;
        }

        /**
         * @brief Deserialize the Sound data
         * 
         * Deserializes the width and height from the provided byte vector.
         * 
         * @param data The byte vector containing serialized data.
         * @throws std::runtime_error If the data size is invalid.
         */
        void deserialize(std::vector<uint8_t> &data) override {
            std::cerr << "Deserializing Sound" << std::endl;

            for (auto &byte : data) {
                std::cerr << (unsigned)byte << " ";
            }
            std::cerr << data.size() << " & " << getSize() << std::endl;
            if (data.size() != getSize())
                throw std::runtime_error("Invalid data size for Sound component");

            std::cerr << "Deserialized sound data size: " << data.size() << std::endl;

            // Copy the string from the vector's first 40 bytes
            sound = std::string(data.begin(), data.begin() + MAX_SOUND_SIZE);
            // Remove trailing null characters (0s)
            sound.erase(std::find(sound.begin(), sound.end(), '\0'), sound.end());
            // Copy the volume from the next byte
            volume = *reinterpret_cast<uint8_t *>(&data[MAX_SOUND_SIZE]);
            // Copy the pitch from the next byte
            pitch = *reinterpret_cast<uint8_t *>(&data[MAX_SOUND_SIZE + sizeof(volume)]);
            // Copy the loop from the next byte
            loop = *reinterpret_cast<uint8_t *>(&data[MAX_SOUND_SIZE + sizeof(volume) + sizeof(pitch)]);
            std::cerr << "Deserialized sound path: " << sound << " volume: " << volume << " pitch: " << pitch << " loop: " << loop << std::endl;
        }

        /**
         * @brief Get the fixed size of the serialized data
         * 
         * @return size_t Size in bytes
         */
        size_t getSize() const override {
            return MAX_SOUND_SIZE + sizeof(volume) + sizeof(pitch) + sizeof(loop);
        }

        /**
         * @brief Adds the Sound component to an entity.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param args The arguments to pass to the component constructor.
         * 
         * @note The arguments should be two uint32_t containing the width and height.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() != 4)
                throw std::runtime_error("Invalid number of arguments for Sound component");

            std::string sound = std::any_cast<std::string>(args[0]);
            uint8_t volume = std::any_cast<uint8_t>(args[1]);
            uint8_t pitch = std::any_cast<uint8_t>(args[2]);
            uint8_t loop = std::any_cast<uint8_t>(args[3]);

            auto Sound = engine.newComponent<Components::Sound>(sound, volume, pitch, loop);
            engine.getRegistry().componentManager().addComponent<Components::Sound>(to, std::move(Sound));
        };

        /**
         * @brief Adds the Sound component to an entity from a configuration setting.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param config The configuration setting to extract the component data from.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            std::string soundVal;
            int volumeVal = 100;
            int pitchVal = 1;
            bool loopVal = false;

            if (!config.lookupValue("path", soundVal)) {
                soundVal = "";
            }
            if (!config.lookupValue("volume", volumeVal)) {
                volumeVal = 100;
            }
            if (!config.lookupValue("pitch", pitchVal)) {
                pitchVal = 100;
            }
            if (!config.lookupValue("loop", loopVal)) {
                loopVal = false;
            }

            std::cerr << "Sound path: " << soundVal << std::endl;
            std::cerr << "Volume: " << volumeVal << std::endl;
            std::cerr << "Pitch: " << pitchVal << std::endl;
            std::cerr << "Loop: " << loopVal << std::endl;
            std::cerr << std::endl;

            attachAndUpdateComponent<Components::Sound>(
                engine, to,
                soundVal,
                static_cast<uint8_t>(volumeVal),
                static_cast<uint8_t>(pitchVal),
                static_cast<uint8_t>(loopVal)
            );
        }

        std::string sound;
        uint8_t volume;
        uint8_t pitch;
        bool loop;

    };
};

#endif // SOUND_HPP
