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
    #include "Texture.hpp"
    #include "Shader.hpp"

namespace Components {
    /**
     * @class Sound
     * 
     * @brief Class that holds a sf::Sound
     */
    class SfmlSoundComponent {
        public:

            SfmlSoundComponent() : __sound(), __buffer(), __loop(false), __hasPlayed(false) {};
            ~SfmlSoundComponent() = default;

            void setSound(const std::string &soundPath)
            {
                if (!__buffer.loadFromFile(soundPath))
                    throw std::runtime_error("Failed to load sound from " + soundPath);
                __sound.setBuffer(__buffer);
            }

            void setVolume(float volume) { __sound.setVolume(volume); }
            void setPitch(float pitch) { __sound.setPitch(pitch); }
            void setLoop(bool loop) { __sound.setLoop(loop); __loop = loop; }

            void play() {
                if (!__loop && __hasPlayed)
                    return;
                if (__sound.getStatus() != sf::Sound::Playing) {
                    __sound.play();
                    __hasPlayed = true;
                }
            }

            void pause() { __sound.pause(); }
            void stop() { __sound.stop(); }

            void setPosition(float x, float y) { __sound.setPosition(x, y, 0); }

            sf::Sound &getSound() { return __sound; }

        private:

            sf::Sound __sound;
            sf::SoundBuffer __buffer;
            bool __loop;
            bool __hasPlayed;

    };
};

#endif // SFML_SOUND_COMPONENT_HPP
