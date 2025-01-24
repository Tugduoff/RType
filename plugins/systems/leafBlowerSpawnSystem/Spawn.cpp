/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Spawn.cpp file
*/

#include "components/position/Position.hpp"
#include "components/type/Type.hpp"
#include "components/collider/Collider.hpp"
#include "components/rotation/Rotation.hpp"
#include "components/score/Score.hpp"

#include "ECS/utilities/Zipper/Zipper.hpp"
#include "Spawn.hpp"
#include "library_entrypoint.hpp"

Systems::Spawn::Spawn()
{
    __speed = 1;
    __maxLeaves = 100;
    __width = 1920;
    __height = 1080;
}

Systems::Spawn::Spawn(libconfig::Setting &config)
{
    unsigned int speed = config.lookup("speed"); // Amount of leaves created each run
    unsigned int maxLeaves = config.lookup("maxLeaves"); // Max amount of leaves on screen
    unsigned int width = config.lookup("width");
    unsigned int height = config.lookup("height");

    __speed = speed;
    __maxLeaves = maxLeaves;
    __width = width;
    __height = height;
}

void Systems::Spawn::init(Engine::GameEngine &engine)
{
    engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position");
    engine.registerComponent<Components::Type>("./plugins/bin/components/", "Type");
    engine.registerComponent<Components::Collider>("./plugins/bin/components/", "Collider");
    engine.registerComponent<Components::Rotation>("./plugins/bin/components/", "Rotation");
    engine.registerComponent<Components::Score>("./plugins/bin/components/", "Score");

    srand(time(NULL));

    // Spawn the player in the middle of the screen, supposedly at 960, 540
    // but can change depending on width and height
    ECS::Entity player = engine.getRegistry().createEntity();

    Components::Position pos(__width / 2, __height / 2, 1); // Player is on layer 1
    Components::Type type(Components::TypeID::ALLY);
    Components::Collider collider(160, 160); // Player is 160x160px
    Components::Rotation rotation(0, 0, 0);
    Components::Score score(0);

    engine.getRegistry().componentManager().addComponent(player, std::make_unique<Components::Position>(pos));
    engine.getRegistry().componentManager().addComponent(player, std::make_unique<Components::Type>(type));
    engine.getRegistry().componentManager().addComponent(player, std::make_unique<Components::Collider>(collider));
    engine.getRegistry().componentManager().addComponent(player, std::make_unique<Components::Rotation>(rotation));
    engine.getRegistry().componentManager().addComponent(player, std::make_unique<Components::Score>(score));

    uint32_t leaves = 0;

    // Spawn starting 10 leaves
    for (int i = 0; i < 10 && leaves < __maxLeaves; i++) {
        spawnLeafAtRandomPosition(engine);
        leaves++;
    }
}

void Systems::Spawn::run(Engine::GameEngine &engine)
{
    auto &componentManager = engine.getRegistry().componentManager();
    auto &typeComponentArr = componentManager.getComponents<Components::Type>();

    uint32_t leaves = 0;

    // Count the number of leaves
    for (auto &&[type] : Zipper(typeComponentArr)) {
        if (type.id == Components::TypeID::ENEMY) {
            leaves++;
        }
    }

    if (leaves < __maxLeaves) {
        for (uint32_t i = 0; i < __speed && leaves < __maxLeaves; i++) { // Spawn leaves based on speed
            spawnLeafAtRandomPosition(engine);
            leaves++;
        }
    }
}

void Systems::Spawn::spawnLeafAtRandomPosition(Engine::GameEngine &engine)
{
    ECS::Entity newLeaf = engine.getRegistry().createEntity();

    uint32_t randomX = rand() % __width;
    uint32_t randomY = rand() % __height;

    Components::Position pos(randomX, randomY, 0); // Leaves are on layer 0
    Components::Type type(Components::TypeID::ENEMY);
    Components::Collider collider(42, 42); // Leaves are 42x42px
    Components::Rotation rotation(0, 0, rand() % 360);

    engine.getRegistry().componentManager().addComponent(newLeaf, std::make_unique<Components::Position>(pos));
    engine.getRegistry().componentManager().addComponent(newLeaf, std::make_unique<Components::Type>(type));
    engine.getRegistry().componentManager().addComponent(newLeaf, std::make_unique<Components::Collider>(collider));
    engine.getRegistry().componentManager().addComponent(newLeaf, std::make_unique<Components::Rotation>(rotation));
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryPoint()
{
    return new Systems::Spawn();
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryConfig(libconfig::Setting &config)
{
    return new Systems::Spawn(config);
}
