/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ConfigLoader.hpp file
*/

#ifndef CONFIGLOADER_HPP
    #define CONFIGLOADER_HPP

    #include "utils/ConfigStruct.hpp"
    #include "plugins/systems/ASystem.hpp"
    #include <string>
    #include <libconfig.h++>
    #include "utils/Chrono.hpp"

namespace Engine {
    class GameEngine;
}

namespace Systems {
    /**
     * @class ConfigLoader
     * 
     * @brief System class from Systems that loads the game configuration.
     * 
     * This system is responsible for loading the game configuration from a file and extracting the data
     */
    class ConfigLoader : public ASystem {
        public:

            ConfigLoader() = default;
            ConfigLoader(const std::string &configFilePath);
            ConfigLoader(libconfig::Setting &config);
            ~ConfigLoader() = default;

            void run(Engine::GameEngine &engine) override;
            void init(Engine::GameEngine &engine) override;

        private:

            void loadConfig(const std::string &filepath);
            void extractConfig(libconfig::Setting &root);
            void displayConfig();
            std::string difficultyToString(enum Difficulty difficulty);

            ConfigStruct config;
            libconfig::Config cfg;
            std::string __configFilePath;
            Chrono __chrono;
    };
};

#endif // CONFIGLOADER_HPP
