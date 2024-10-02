/*
** EPITECH PROJECT, 2024
** RType Tests
** File description:
** Main.cpp tests file
*/

#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/redirect.h>
#include "GameEngine.hpp"
#include "ECS/entity/Entity.hpp"
#include "ECS/registry/Registry.hpp"
#include "plugins/components/position/Position.hpp"
#include "ECS/utilities/SparseArray.hpp"

// Redirect standard output so we can check the output of the program
void redirect_all_stdout(void) {
    cr_redirect_stdout();
}

// Test for registering and adding a component to an entity
Test(ComponentManager, register_and_add_component, .init = redirect_all_stdout)
{
    Engine::GameEngine gameEngine;
    ECS::Registry &reg = gameEngine.getRegistry();
    std::string positionPluginPath = "./plugins/bin/components/Position.so";

    // Manually test for exceptions instead of using cr_assert_no_throw
    bool exception_thrown = false;

    try {
        // Register Position component
        gameEngine.registerComponent<Components::Position>(positionPluginPath);

        // Create an entity
        ECS::Entity entity = reg.entityManager().spawnEntity();

        // Load a Position component
        std::unique_ptr<Components::Position> position = gameEngine.newComponent<Components::Position>(10, 20);

        // Add the Position component to the entity
        reg.componentManager().addComponent<Components::Position>(entity, std::move(position));
    } catch (const std::exception &e) {
        exception_thrown = true;
    }

    // Ensure no exception was thrown
    cr_assert_not(exception_thrown, "An exception was thrown when registering or adding a component.");
}

// Test for handling a missing component registration
Test(ComponentManager, load_unregistered_component)
{
    Engine::GameEngine gameEngine;

    bool exception_thrown = false;
    try {
        // Attempt to load a component without registering it
        std::unique_ptr<Components::Position> position = gameEngine.newComponent<Components::Position>(10, 20);
    } catch (const std::runtime_error &e) {
        exception_thrown = true;
        // Optional: Verify the exception message if you want to check its content
        unsigned int strSize = strlen(e.what());
        cr_assert_gt(strSize, 10, "Exception msg size is not greater than 10, that is the required size defined for our exceptions.");
    }

    // Assert that the exception was thrown
    cr_assert(exception_thrown, "No exception was thrown when trying to load an unregistered component.");
}

// Test for adding a component to an entity
Test(ComponentManager, add_component)
{
    Engine::GameEngine gameEngine;
    ECS::Registry &reg = gameEngine.getRegistry();
    std::string positionPluginPath = "./plugins/bin/components/Position.so";

    try {
        // Register the Position component
        gameEngine.registerComponent<Components::Position>(positionPluginPath);

        // Create an entity
        ECS::Entity entity = reg.entityManager().spawnEntity();

        // Load and add a Position component to the entity
        std::unique_ptr<Components::Position> position = gameEngine.newComponent<Components::Position>(10, 20);

        // Add the Position component to the entity
        reg.componentManager().addComponent<Components::Position>(entity, std::move(position));

        // Get the SparseArray for Position components
        SparseArray<Components::Position> &positionComponents = reg.componentManager().getComponents<Components::Position>();

        // Check if the component was added to the entity using the overloaded operator size_t
        cr_assert_not_null(positionComponents[entity], "Position component was not added to the entity.");
        cr_assert_eq(positionComponents[entity]->x, 10, "Position x coordinate is incorrect.");
        cr_assert_eq(positionComponents[entity]->y, 20, "Position y coordinate is incorrect.");
    } catch (const std::runtime_error &e) {
        cr_assert_fail("Unexpected runtime error: %s", e.what());
    }
}

// Test for removing adding a component and then removing it from an entity
Test(ComponentManager, remove_component)
{
    Engine::GameEngine gameEngine;
    ECS::Registry &reg = gameEngine.getRegistry();
    std::string positionPluginPath = "./plugins/bin/components/Position.so";

    try {
        // Register the Position component
        gameEngine.registerComponent<Components::Position>(positionPluginPath);

        // Create an entity
        ECS::Entity entity = reg.entityManager().spawnEntity();

        // Load and add a Position component to the entity
        std::unique_ptr<Components::Position> position = gameEngine.newComponent<Components::Position>(10, 20);

        // Add the Position component to the entity
        reg.componentManager().addComponent<Components::Position>(entity, std::move(position));

        // Remove the Position component from the entity
        reg.componentManager().removeComponent<Components::Position>(entity);

        // Get the SparseArray for Position components
        SparseArray<Components::Position> &positionComponents = reg.componentManager().getComponents<Components::Position>();

        // Check if the component was removed from the entity
        cr_assert_null(positionComponents[entity], "Position component was not removed from the entity.");
    } catch (const std::runtime_error &e) {
        cr_assert_fail("Unexpected runtime error: %s", e.what());
    }
}
