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
        static constexpr std::size_t MAX_ID_SIZE = 20; // Fixed size for network
        static constexpr std::size_t MAX_SOUND_SIZE = 40; // Fixed size for network

        /**
         * @brief Default constructor
         * 
         * Initializes the Sound with default width and height of 1.
         */
        Sound(std::vector<std::tuple<std::string, std::string, uint8_t, uint8_t, bool, bool>> sounds = {
            std::make_tuple("NONE", "", 100, 1, false, false)
        })
            : AComponent(std::string("Sound")), sounds(sounds) {}

        /**
         * @brief Constructor using configuration.
         * 
         * Initializes the Sound based on config settings, with default values
         */
        Sound(libconfig::Setting &config) : AComponent(std::string("Sound")) {
            const libconfig::Setting &sounds = config.lookup("sounds");
            std::vector<std::tuple<std::string, std::string, uint8_t, uint8_t, bool, bool>> soundsVec;

            for (int i = 0; i < sounds.getLength(); i++) {
                libconfig::Setting &sound = sounds[i];
                std::string soundIdVal = "AMBIENT";
                std::string pathVal = "";
                int volumeVal = 100;
                int pitchVal = 1;
                bool loopVal = false;

                sound.lookupValue("id", soundIdVal);
                sound.lookupValue("path", pathVal);
                sound.lookupValue("volume", volumeVal);
                sound.lookupValue("pitch", pitchVal);
                sound.lookupValue("loop", loopVal);

                std::string newId = std::string('\0', MAX_ID_SIZE);
                std::copy(soundIdVal.begin(), soundIdVal.end(), newId.begin());

                std::string newPath = std::string('\0', MAX_SOUND_SIZE);
                std::copy(pathVal.begin(), pathVal.end(), newPath.begin());

                soundsVec.push_back(std::make_tuple(newId, newPath, volumeVal, pitchVal, loopVal, false));

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
            std::vector<uint8_t> data(getSize());
            data[0] = sounds.size();

            for (size_t i = 0; i < sounds.size(); i++) {
                unsigned int offset = i * (MAX_SOUND_SIZE + MAX_ID_SIZE + 4) + 1;
                std::string id = std::get<0>(sounds[i]);
                std::string sound = std::get<1>(sounds[i]);
                uint8_t volume = std::get<2>(sounds[i]);
                uint8_t pitch = std::get<3>(sounds[i]);
                uint8_t loop = std::get<4>(sounds[i]);
                uint8_t state = std::get<5>(sounds[i]);

                // Copy the sound id and path into the data
                std::string newId;
                newId.resize(MAX_ID_SIZE);
                std::copy(id.begin(), id.end(), newId.begin());

                std::string newPath;
                newPath.resize(MAX_SOUND_SIZE);
                std::copy(sound.begin(), sound.end(), newPath.begin());

                std::copy(newId.begin(), newId.end(), data.begin() + offset);
                std::copy(newPath.begin(), newPath.end(), data.begin() + offset + MAX_ID_SIZE);

                // Copy the volume into the next byte
                data[offset + MAX_SOUND_SIZE + MAX_ID_SIZE] = volume;
                // Copy the pitch into the next byte
                data[offset + MAX_SOUND_SIZE + MAX_ID_SIZE + 1] = pitch;
                // Copy the loop into the next byte
                data[offset + MAX_SOUND_SIZE + MAX_ID_SIZE + 2] = loop;
                // Copy the state into the next byte
                data[offset + MAX_SOUND_SIZE + MAX_ID_SIZE + 3] = state;
            }
            // std::cerr << "Serialized Sound:" << std::endl;
            // for (auto &byte : data) {
            //     std::cerr << (unsigned)byte << " ";
            // }
            // std::cerr << std::endl;
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
            // std::cerr << "Deserializing Sound" << std::endl;
            std::vector<std::tuple<std::string, std::string, uint8_t, uint8_t, bool, bool>> newSounds;

            // std::cerr << "Deserialized Sound:" << std::endl;
            // for (auto &byte : data) {
            //     std::cerr << (unsigned)byte << " ";
            // }
            // std::cerr << std::endl;
            uint8_t size = data[0];

            for (int i = 0; i < size; i++) {
                unsigned int offset = i * (MAX_SOUND_SIZE + MAX_ID_SIZE + 4) + 1;
                std::string id = std::string(data.begin() + offset, data.begin() + offset + MAX_ID_SIZE);
                std::string sound = std::string(data.begin() + offset + MAX_ID_SIZE, data.begin() + offset + MAX_ID_SIZE + MAX_SOUND_SIZE);
                uint8_t volume = data[offset + MAX_SOUND_SIZE + MAX_ID_SIZE];
                uint8_t pitch = data[offset + MAX_SOUND_SIZE + MAX_ID_SIZE + 1];
                uint8_t loop = data[offset + MAX_SOUND_SIZE + MAX_ID_SIZE + 2];
                uint8_t state = data[offset + MAX_SOUND_SIZE + MAX_ID_SIZE + 3];

                id.erase(std::find(id.begin(), id.end(), '\0'), id.end());
                sound.erase(std::find(sound.begin(), sound.end(), '\0'), sound.end());
                newSounds.push_back(std::make_tuple(id, sound, volume, pitch, loop, state));
            }
            sounds = newSounds;
        }

        /**
         * @brief Get the fixed size of the serialized data
         * 
         * @return size_t Size in bytes
         */
        size_t getSize() const override { return sounds.size() * (MAX_SOUND_SIZE + MAX_ID_SIZE + 4) + 1; }

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
            if (args.size() != 1)
                throw std::runtime_error("Invalid number of arguments for Sound component");

            std::vector<std::tuple<std::string, std::string, uint8_t, uint8_t, bool, bool>> soundsVec = std::any_cast<std::vector<std::tuple<std::string, std::string, uint8_t, uint8_t, bool, bool>>>(args[0]);

            attachAndUpdateComponent<Components::Sound>(
                engine, to,
                soundsVec
            );
        };

        /**
         * @brief Adds the Sound component to an entity from a configuration setting.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param config The configuration setting to extract the component data from.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            const libconfig::Setting &sounds = config.lookup("sounds");
            std::vector<std::tuple<std::string, std::string, uint8_t, uint8_t, bool, bool>> soundsVec;

            for (int i = 0; i < sounds.getLength(); i++) {
                libconfig::Setting &sound = sounds[i];

                std::string soundIdVal = "AMBIENT";
                std::string pathVal = "";
                int volumeVal = 100;
                int pitchVal = 1;
                bool loopVal = false;

                sound.lookupValue("id", soundIdVal);
                sound.lookupValue("path", pathVal);
                sound.lookupValue("volume", volumeVal);
                sound.lookupValue("pitch", pitchVal);
                sound.lookupValue("loop", loopVal);

                // std::cerr << "Sound before process: " << soundIdVal << " " << pathVal << " " << volumeVal << " " << pitchVal << " " << loopVal << std::endl;

                soundsVec.push_back(std::make_tuple(soundIdVal, pathVal, volumeVal, pitchVal, loopVal, false));

            }
            // for (auto &sound : soundsVec) {
            //     std::cerr << "Sound: " << std::get<0>(sound) << " " << std::get<1>(sound) << " " << std::get<2>(sound) << " " << std::get<3>(sound) << " " << std::get<4>(sound) << std::endl;
            // }
            attachAndUpdateComponent<Components::Sound>(
                engine, to,
                soundsVec
            );
        }

        // type of the sound, path to the sound, volume, pitch, sound loops, state
        std::vector<std::tuple<std::string, std::string, uint8_t, uint8_t, bool, bool>> sounds;

    };
};

#endif // SOUND_HPP
