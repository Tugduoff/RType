/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ConfigLoader.hpp file
*/

#ifndef CONFIGLOADER_HPP
    #define CONFIGLOADER_HPP

    #include "ECS/config/ConfigStruct.hpp"
    #include "plugins/systems/AServerSystem.hpp"
    #include <string>
    #include <libconfig.h++>

namespace Systems {
    class ConfigLoader : public AServerSystem {
        public:

            ConfigLoader() = default;
            ConfigLoader(const std::string &configFilePath);
            ConfigLoader(libconfig::Setting &config);
            ~ConfigLoader() = default;

            void run(Engine::ServerEngine &engine) override;
            void init(Engine::ServerEngine &engine) override;

        private:

            void loadConfig(const std::string &filepath, Engine::ServerEngine &engine);
            void extractConfig(libconfig::Setting &root, Engine::ServerEngine &engine);
            void displayConfig();
            std::string difficultyToString(enum Difficulty difficulty);

            ConfigStruct config;
            libconfig::Config cfg;
            std::string __configFilePath;
    };
};

#endif // CONFIGLOADER_HPP
