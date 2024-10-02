/*
** EPITECH PROJECT, 2024
** RType
** File description:
** GameEngine.cpp file
*/

#include "GameEngine.hpp"
#include "ECS/component/ComponentRegisterer.hpp"
#include <libconfig.h++>
#include <typeindex>

void Engine::GameEngine::loadSystems(const std::string &systemsFolderPath, const std::string &systemsConfigFile)
{
    libconfig::Config cfg;

    try {
        cfg.readFile(systemsConfigFile);
        libconfig::Setting &root = cfg.getRoot();
        if (root.exists("systems")) {
            libconfig::Setting &systems = root["systems"];
            for (int i = 0; i < systems.getLength(); ++i) {
                std::string systemName = systems[i].c_str();
                std::string systemPath = systemsFolderPath + systemName + ".so";
                DLLoader loader(systemPath);
                std::unique_ptr<Systems::ISystem> system = loader.getInstance<Systems::ISystem>("entryPoint", loader);
                __registry.systemManager().addSystem(std::move(system));
                __systemLoaders.push_back(std::move(loader));
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
    __registry.systemManager().initSystems(ECS::ComponentRegisterer(
        this->getRegistry().componentManager(),
        [this](auto lib, auto idx) { return _loadComponentLib(lib, idx); }
    ));
}

void Engine::GameEngine::runSystems()
{
    __registry.systemManager().run(*this);
}

bool Engine::GameEngine::_loadComponentLib(const std::string &libName, std::type_index typeIndex)
{
    if (__componentLoaders.contains(typeIndex))
        return (false);

    __componentLoaders.emplace(typeIndex, DLLoader(libName));

    return true;
}
