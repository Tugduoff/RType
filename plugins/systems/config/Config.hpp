/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ConfigLoader.hpp file
*/

#ifndef CONFIGLOADER_HPP
    #define CONFIGLOADER_HPP

    #include "ECS/config/ConfigStruct.hpp"
    #include "plugins/systems/ASystem.hpp"
    #include <string>
    #include <libconfig.h++>

namespace Engine {
    class GameEngine;
}

namespace Systems {
    class ConfigLoader : public ASystem {
        public:

            ConfigLoader() = default;
            ConfigLoader(const std::string &configFilePath);
            ~ConfigLoader() = default;

            void run(Engine::GameEngine &engine) override;
            void init(Engine::GameEngine &engine) override;

        private:

            void loadConfig(const std::string &filepath, Engine::GameEngine &engine);
            void extractConfig(libconfig::Setting &root, Engine::GameEngine &engine);
            void displayConfig();
            std::string difficultyToString(enum Difficulty difficulty);

            ConfigStruct config;
            libconfig::Config cfg;
            std::string __configFilePath;
    };
};

#endif // CONFIGLOADER_HPP
