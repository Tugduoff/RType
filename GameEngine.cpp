/*
** EPITECH PROJECT, 2024
** RType
** File description:
** GameEngine.cpp file
*/

#include "GameEngine.hpp"
#include <libconfig.h++>

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
                    systemPath += systemName + ".so";

                    std::cout << "Loading system: " << systemPath << std::endl;
                    std::vector<std::any> instanceArgs;
                    if (systemConfig.exists("args")) {
                        libconfig::Setting &args = systemConfig["args"];
                        for (int k = 0; k < args.getLength(); ++k) {
                            libconfig::Setting &arg = args[k];
                            if (arg.getType() == libconfig::Setting::TypeInt) {
                                instanceArgs.push_back(arg.operator int());
                            } else if (arg.getType() == libconfig::Setting::TypeFloat) {
                                instanceArgs.push_back(arg.operator double());
                            } else if (arg.getType() == libconfig::Setting::TypeString) {
                                instanceArgs.push_back(arg.c_str());
                            } else if (arg.getType() == libconfig::Setting::TypeBoolean) {
                                instanceArgs.push_back(arg.operator bool());
                            } else {
                                std::cerr << "Unsupported argument type" << std::endl;
                            }
                        }
                    }
                    DLLoader loader(systemPath);
                    std::unique_ptr<Systems::ISystem> system = loader.getInstance<Systems::ISystem>("entryPoint", instanceArgs);
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
