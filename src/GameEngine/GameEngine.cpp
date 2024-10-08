/*
** EPITECH PROJECT, 2024
** RType
** File description:
** GameEngine.cpp file
*/

#include <libconfig.h++>
#include "GameEngine.hpp"

void Engine::GameEngine::loadSystems(const std::string &systemsConfigFile)
{
    libconfig::Config cfg;

    try {
        cfg.readFile(systemsConfigFile);
        libconfig::Setting &root = cfg.getRoot();
        if (root.exists("systems")) {
            libconfig::Setting &systems = root["systems"];
            std::string systemsFolderPath = "./plugins/bin/systems/";
            for (int i = 0; i < systems.getLength(); ++i) {
                libconfig::Setting &systemConfig = systems[i];
                std::string systemPath = systemsFolderPath;
                std::string dirName;
                std::string systemName;

                if (systemConfig.lookupValue("dir", dirName)) {
                    systemPath += dirName;
                }
                if (systemConfig.lookupValue("name", systemName)) {
                    std::cout << "Loading system: " << systemPath + systemName << std::endl;
                    libconfig::Setting &args = systemConfig["args"];

                    DLLoader loader(systemPath, systemName);
                    std::unique_ptr<Systems::ISystem> system = loader.getUniqueInstance<Systems::ISystem, const libconfig::Setting &>("entryConfig", args);
                    __registry.systemManager().addSystem(std::move(system));
                    __systemLoaders.push_back(std::move(loader));
                }
            }
            initSystems();
        }
    } catch (libconfig::ParseException &e) {
        std::cerr << "Error while parsing file: "
            << e.getFile() << " in line: "
            << e.getLine() << " : "
            << e.getError() << std::endl;
    } catch (libconfig::FileIOException &e) {
        std::cerr << "Error while reading file: "
            << e.what() << std::endl;
    }
}

void Engine::GameEngine::initSystems()
{
    __registry.systemManager().initSystems(*this);
}

void Engine::GameEngine::runSystems()
{
    __registry.systemManager().run(*this);
}
