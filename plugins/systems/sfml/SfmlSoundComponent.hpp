/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Sound.hpp file
*/

#ifndef SFML_SOUND_COMPONENT_HPP
    #define SFML_SOUND_COMPONENT_HPP

    #include <SFML/Graphics.hpp>
    #include <SFML/Audio.hpp>
    #include <libconfig.h++>
    #include "GameEngine/GameEngine.hpp"
    #include <unordered_map>
    #include <vector>
    #include <tuple>
    #include "Texture.hpp"
    #include "Shader.hpp"
    #include <memory>

namespace Components {
    /**
     * @class Sound
     * 
     * @brief Class that holds a sf::Sound
     */
    class SfmlSoundComponent {
        public:
            SfmlSoundComponent() = default;
            ~SfmlSoundComponent() = default;

            void addSound(const std::string &id, const std::string &soundPath, float volume, float pitch, bool loop) {
                // Create shared pointers for SoundBuffer and Sound
                auto buff = std::make_shared<sf::SoundBuffer>();
                auto sound = std::make_shared<sf::Sound>();

                // Load the buffer from the file and set up the sound
                if (!buff->loadFromFile(soundPath)) {
                    throw std::runtime_error("Failed to load sound from " + soundPath);
                }

                // Associate buffer with sound and set properties
                sound->setBuffer(*buff);
                sound->setVolume(volume);
                sound->setPitch(pitch);
                sound->setLoop(loop);

                std::cerr << "Adding sound: " << id << std::endl;

                __sounds.emplace_back(id, sound, buff, volume, pitch, loop, false);
            }

            void play(const std::string &id) {
                std::cerr << "Playing sound in play: " << id << std::endl;
                for (auto &sound : __sounds) {
                    if (std::get<0>(sound) == id) {
                        std::cerr << "Playing sound: " << std::get<0>(sound) << std::endl;
                        std::cerr << "Loop: " << std::get<5>(sound) << std::endl;
                        std::cerr << "Has played: " << std::get<6>(sound) << std::endl;

                        // Skip if sound is non-looping and has already played
                        if (!std::get<5>(sound) && std::get<6>(sound))
                            return;

                        // Only play sound if it isn't already playing
                        if (std::get<1>(sound)->getStatus() != sf::Sound::Playing) {
                            std::cerr << "Sound is not playing, playing sound: " << std::get<0>(sound) << std::endl;
                            std::cerr << "Volume: " << std::get<3>(sound) << std::endl;
                            std::get<1>(sound)->play();
                            std::get<6>(sound) = true; // Mark sound as played
                        }
                    }
                }
            }

            bool hasSound(const std::string &id) {
                for (auto &sound : __sounds) {
                    if (std::get<0>(sound) == id) {
                        return true;
                    }
                }
                return false;
            }

            void reset(const std::string &id) {
                for (auto &sound : __sounds) {
                    if (std::get<0>(sound) == id) {
                        std::cerr << "Resetting sound: " << std::get<0>(sound) << std::endl;
                        std::get<1>(sound)->stop();
                        std::get<6>(sound) = false; // Mark sound as not played
                    }
                }
            }

        private:
            std::vector<std::tuple<std::string, std::shared_ptr<sf::Sound>, std::shared_ptr<sf::SoundBuffer>, float, float, bool, bool>> __sounds;
    };
};

#endif // SFML_SOUND_COMPONENT_HPP
