/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Collision.cpp file
*/

#include "components/position/Position.hpp"
#include "components/type/Type.hpp"
#include "components/collider/Collider.hpp"
#include "components/score/Score.hpp"

#include "ECS/utilities/Zipper/Zipper.hpp"
#include "ECS/utilities/Zipper/IndexedZipper.hpp"
#include "Collision.hpp"
#include "library_entrypoint.hpp"
#include "raylib.h"

Systems::Collision::Collision()
{
    __playerForce = 1;
}

Systems::Collision::Collision(libconfig::Setting &config)
{
    unsigned int playerForce = config.lookup("playerForce"); // The force of the player (default 1)

    __playerForce = playerForce;
}

void Systems::Collision::init(Engine::GameEngine &engine)
{
    engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position");
    engine.registerComponent<Components::Type>("./plugins/bin/components/", "Type");
    engine.registerComponent<Components::Collider>("./plugins/bin/components/", "Collider");
    engine.registerComponent<Components::Score>("./plugins/bin/components/", "Score");
}

void Systems::Collision::run(Engine::GameEngine &engine)
{
    auto &componentManager = engine.getRegistry().componentManager();
    auto &posComponentArr = componentManager.getComponents<Components::Position>();
    auto &typeComponentArr = componentManager.getComponents<Components::Type>();
    auto &colliderComponentArr = componentManager.getComponents<Components::Collider>();
    auto &scoreComponentArr = componentManager.getComponents<Components::Score>();

    std::unique_ptr<ECS::Entity> player;
    std::vector<ECS::Entity> leaves;

    for (auto &&[i, type] : IndexedZipper(typeComponentArr)) {
        switch (type.id) {
            case Components::TypeID::ALLY:
                player = std::make_unique<ECS::Entity>(i);
                break;
            case Components::TypeID::ENEMY:
                leaves.push_back((ECS::Entity)i);
                break;
            default:
                break;
        }
    }

    if (!player) {
        std::cerr << "No player found" << std::endl;
        return;
    }

    // Check the collision between the player collider and the leaf collider for each leaf in leaves
    auto &playerCollider = colliderComponentArr[*player];
    auto &playerPos = posComponentArr[*player];
    auto &playerScore = scoreComponentArr[*player];

    int posX = playerPos->x - 80;
    int posY = playerPos->y - 80;

    // DrawRectangle(posX, posY, playerCollider->width, playerCollider->height, RED);

    for (auto &leaf : leaves) {
        auto &leafPos = posComponentArr[leaf];
        auto &leafCollider = colliderComponentArr[leaf];

        int leafPosX = leafPos->x;
        int leafPosY = leafPos->y;

        if (leafPosX < posX + (int)playerCollider->width &&
            leafPosX + (int)leafCollider->width > posX &&
            leafPosY < posY + (int)playerCollider->height &&
            leafPosY + (int)leafCollider->height > posY) {
            playerScore->value += 1;
            engine.getRegistry().killEntity(leaf);
        }
    }
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryPoint()
{
    return new Systems::Collision();
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryConfig(libconfig::Setting &config)
{
    return new Systems::Collision(config);
}
