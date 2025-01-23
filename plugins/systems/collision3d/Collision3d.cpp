/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Collision3d.cpp file
*/

#include "ECS/utilities/Zipper/IndexedZipper.hpp"
#include "GameEngine/GameEngine.hpp"
#include "Collision3d.hpp"
#include "components/position3d/Position3d.hpp"
#include "components/collider3d/Collider3d.hpp"
#include "components/marioType/MarioType.hpp"
#include "components/damage/Damage.hpp"
#include "components/score/Score.hpp"
#include "components/sound/Sound.hpp"
#include "components/destruction/Destruction.hpp"
#include "library_entrypoint.hpp"
#include <iostream>
#include <stdexcept>

Systems::Collision3d::Collision3d(libconfig::Setting &)
{
}

void Systems::Collision3d::run(Engine::GameEngine &engine)
{
    try {
        checkPlayerToItemBoxCollision3d(engine);
    } catch (std::runtime_error &e) {
        std::cerr << "Collision3d Error: " << e.what() << std::endl;
    }
}

void Systems::Collision3d::init(Engine::GameEngine &engine)
{
    engine.registerComponent<Components::Position3d>("./plugins/bin/components/", "Position3d");
    engine.registerComponent<Components::Collider3d>("./plugins/bin/components/", "Collider3d");
    engine.registerComponent<Components::MarioType>("./plugins/bin/components/", "MarioType");
    engine.registerComponent<Components::Score>("./plugins/bin/components/", "Score");
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryPoint()
{
    return new Systems::Collision3d();
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryConfig(libconfig::Setting &config)
{
    return new Systems::Collision3d(config);
}

void Systems::Collision3d::checkPlayerToItemBoxCollision3d(Engine::GameEngine &engine)
{
    auto &reg = engine.getRegistry();

    auto &typeArr = reg.componentManager().getComponents<Components::MarioType>();
    auto &posArr = reg.componentManager().getComponents<Components::Position3d>();
    auto &colliderArr = reg.componentManager().getComponents<Components::Collider3d>();
    auto &scoreArr = reg.componentManager().getComponents<Components::Score>();

    std::vector<ECS::Entity> players;
    std::vector<ECS::Entity> itemBoxes;

    for (auto &&[i, type] : IndexedZipper(typeArr)) {
        if (type.id == Components::MarioTypeID::PLAYER) {
            players.push_back((ECS::Entity)i);
        } else if (type.id == Components::MarioTypeID::ITEM_BOX) {
            itemBoxes.push_back((ECS::Entity)i);
        }
    }

    // std::cerr << "Players: " << players.size() << " ; ItemBoxes: " << itemBoxes.size() << std::endl;

    for (auto &box : itemBoxes) {
        for (auto &player : players) {
            try {
                auto &boxPos = posArr[box];
                auto &boxCollider = colliderArr[box];
                auto &playerPos = posArr[player];
                auto &playerCollider = colliderArr[player];

                if (boxPos->x + boxCollider->width >= playerPos->x &&
                    boxPos->x <= playerPos->x + playerCollider->width &&
                    boxPos->y + boxCollider->height >= playerPos->y &&
                    boxPos->y <= playerPos->y + playerCollider->height &&
                    boxPos->z + boxCollider->depth >= playerPos->z &&
                    boxPos->z <= playerPos->z + playerCollider->depth) {
                    auto &playerScore = scoreArr[player];
                    auto &boxScore = scoreArr[box];

                    playerScore->value += boxScore->value;

                    std::cerr << "Player: " << player << " hit item_box: " << box << " ; New player score: " << playerScore->value << std::endl;
                    reg.killEntity(box);
                }
            } catch (std::exception &e) {
                continue;
            }
        }
    }
}
