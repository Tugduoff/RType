/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ActionManager.cpp file
*/

#include "ActionManager.hpp"
#include "components/action/Action.hpp"
#include "SpriteComponent.hpp"

Systems::ActionManager::ActionManager()
{
}

Systems::ActionManager::ActionManager(libconfig::Setting &)
{
}

void Systems::ActionManager::init(Engine::GameEngine &engine)
{
    auto &manager = engine.getRegistry().componentManager();

    if (!engine.registerComponent<Components::ActionComponent>("./plugins/bin/components/", "Action"))
        std::cerr << "Error: Could not register Action component in system ActionManager" << std::endl;
    auto ctor = []() -> Components::SpriteComponent * { return new Components::SpriteComponent(); };
    if (!manager.registerComponent<Components::SpriteComponent>(ctor))
        std::cerr << "Error: Could not register SpriteComponent component in system ActionManager" << std::endl;
}

void Systems::ActionManager::run(Engine::GameEngine &engine)
{
    try {
        // Retrieve Sprite and Action components from the engine's registry
        auto &spriteComponents = engine.getRegistry().componentManager().getComponents<Components::SpriteComponent>();
        auto &actionComponents = engine.getRegistry().componentManager().getComponents<Components::ActionComponent>();

        size_t i = 0;

        // Iterate through both sprite and action components
        for (; i < actionComponents.size() && i < spriteComponents.size(); i++) {
            try {
                auto &sprite = spriteComponents[i]; // Get the sprite component for the entity

                try {
                    auto &actionComponent = actionComponents[i]; // Get the action component for the entity

                    // If either the action or sprite component is missing, skip to the next entity
                    if (!actionComponent || !sprite)
                        continue;

                    // Get the current and next actions from the sprite
                    enum EntityAction currentAction = stringToEntityAction(sprite->currentAction);
                    enum EntityAction nextAction = stringToEntityAction(sprite->nextTexture);

                    if (currentAction != actionComponent->action && nextAction != actionComponent->action) {
                        sprite->loadTextureForAction(actionToString(actionComponent->action));
                    }

                } catch (std::exception &e) {
                    // Handle the case where the action component does not exist or is invalid
                    std::unique_ptr<Components::ActionComponent> actionComp = std::make_unique<Components::ActionComponent>();
                    engine.getRegistry().componentManager().addComponent<Components::ActionComponent>((ECS::Entity)i, std::move(actionComp));
                    // std::cerr << "Set default action for entity: " << i << " in ActionManager." << std::endl;
                    continue;
                }
            } catch (std::exception &) {
                continue; // Continue if sprite component or entity has issues
            }
        }
    } catch (std::exception &e) {
        // Handle any general exceptions encountered in the ActionManager
        std::cerr << "Error while running ActionManager: " << e.what() << std::endl;
    }
}
