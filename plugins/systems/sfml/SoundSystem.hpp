/*
** EPITECH PROJECT, 2024
** RType
** File description:
** SoundSystem.hpp file
*/

#ifndef SOUND_SYSTEM_HPP
    #define SOUND_SYSTEM_HPP

    #include <SFML/Graphics.hpp>
    #include <libconfig.h++>
    #include "GameEngine/GameEngine.hpp"
    #include <unordered_map>
    #include <vector>
    #include "Texture.hpp"
    #include "Shader.hpp"

namespace Systems {
    /**
     * @class Sound
     * 
     * @brief System class from Systems that Sounds entities.
     * 
     * This system is responsible for Sounding entities on the screen.
     */
    class SoundSystem {
        public:

            SoundSystem();
            SoundSystem(libconfig::Setting &config);
            ~SoundSystem() = default;

            void run(Engine::GameEngine &engine);
            void init(Engine::GameEngine &engine);

        private:

    };
};

#endif // SOUND_SYSTEM_HPP
