/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Main.cpp file
*/


#include <exception>
#include <iostream>
#include <typeindex>
#include <vector>
#include "GameEngine/GameEngine.hpp"
#include "ECS/registry/Registry.hpp"
#include "utils/Chrono.hpp"
#include "components/IComponent.hpp"

template<typename It>
void displayPolymorphic(Engine::GameEngine &engine, It begin, It end)
{
    int i = 0;

    std::cout << std::endl;
    for (auto it = begin; it != end; ++it) {
        std::type_index &idx = *it;

        std::cout << "For type index {" << idx.name() << "}:\n" << std::endl;

        auto &dummyComp = engine.getComponentFromId(idx);

        auto &sparseArray = dummyComp->any_cast(
            engine.getRegistry().componentManager().getComponents(idx)
        );

        for (auto const &comp : sparseArray) {
            if (!comp)
                continue;
            std::cout << "    " << comp->getId() << ": {";
            i = 0;
            for (const auto &byte : comp->serialize()) {
                if (i != 0)
                    std::cout << ", ";
                std::cout << (unsigned)byte;
                i++;
            }
            std::cout << "}" << std::endl;
        }
        std::cout << std::endl;
    }
}

void updateComponent(size_t id, std::string name, std::vector<uint8_t> data)
{
    std::cout << "Updating component: " << name << " with ID: " << id << std::endl;
    std::cout << "    {";
    int i = 0;
    for (const auto &byte : data) {
        if (i != 0)
            std::cout << ", ";
        std::cout << (unsigned)byte;
        i++;
    }
    std::cout << "}" << std::endl;
}

int main() {
    Engine::GameEngine engine(
        []([[maybe_unused]]size_t id, [[maybe_unused]]std::string name, [[maybe_unused]]std::vector<uint8_t> data) {}
    );
    Chrono chrono;

    try {
        engine.loadSystems("./src/server/configServer.cfg");

        while (true) {
            if (chrono.getElapsedTime() < 17)
                continue;
            engine.runSystems();
            chrono.restart();
       }

    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
