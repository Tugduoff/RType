/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Main.cpp file
*/

#include "GameEngine/GameEngine.hpp"
#include "ECS/registry/Registry.hpp"
#include "ECS/utilities/SparseArray.hpp"
#include "plugins/components/IComponent.hpp"
#include "plugins/components/position/Position.hpp"
#include "plugins/components/velocity/Velocity.hpp"
#include "plugins/components/spriteId/SpriteID.hpp"
#include "plugins/systems/sfml/SpriteComponent.hpp"
#include "Chrono.hpp"
#include <exception>
#include <iostream>

void displayComponents(ECS::Registry &reg)
{
    SparseArray<Components::Position> &positionComponents = reg.componentManager().getComponents<Components::Position>();
    SparseArray<Components::Velocity> &velocityComponents = reg.componentManager().getComponents<Components::Velocity>();
    SparseArray<Components::SpriteIDComponent> &spriteIDComponents = reg.componentManager().getComponents<Components::SpriteIDComponent>();

    std::cerr << "Displaying components: " << std::endl;
    std::cerr << "\nPosition components: \n" << std::endl;
    for (std::unique_ptr<Components::Position> &pos : positionComponents) {
        if (!pos)
            continue;
        std::cerr << "Position: " << pos->x << ", " << pos->y << std::endl;
    }

    std::cerr << "\nVelocity components: \n" << std::endl;
    for (std::unique_ptr<Components::Velocity> &vel : velocityComponents) {
        if (!vel)
            continue;
        std::cerr << "Velocity: " << vel->x << ", " << vel->y << std::endl;
    }

    std::cerr << "\nSpriteID components: \n" << std::endl;
    for (std::unique_ptr<Components::SpriteIDComponent> &spriteID : spriteIDComponents) {
        if (!spriteID)
            continue;
        std::cerr << "SpriteID: " << Components::SpriteIDToString.at(spriteID->id) << std::endl;
    }
}

void updateComponent(size_t, std::string, std::vector<uint8_t>)
{
}

int main() {
    Engine::GameEngine engine(updateComponent);
    ECS::Registry &reg = engine.getRegistry();
    ECS::Entity entity = reg.entityManager().spawnEntity();
    Chrono chrono;
    try {
        engine.loadSystems("./plugins/bin/systems/configSystems.cfg");

        std::unique_ptr<Components::Position> position = engine.newComponent<Components::Position>(10, 20, 1);
        std::unique_ptr<Components::SpriteIDComponent> spriteId = engine.newComponent<Components::SpriteIDComponent>(Components::SpriteID::Player);
        std::unique_ptr<Components::SpriteComponent> sprite = std::make_unique<Components::SpriteComponent>();
        std::unique_ptr<Components::Velocity> velocity = engine.newComponent<Components::Velocity>(1, 0);

        reg.componentManager().addComponent<Components::Position>(entity, std::move(position));
        reg.componentManager().addComponent<Components::SpriteIDComponent>(entity, std::move(spriteId));
        reg.componentManager().addComponent<Components::SpriteComponent>(entity, std::move(sprite));
        reg.componentManager().addComponent<Components::Velocity>(entity, std::move(velocity));

        // displayComponents(reg);

        while (true) {
            if (chrono.getElapsedTime() > 25) {
                engine.runSystems();
                chrono.restart();
            }
        }
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
