#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/redirect.h>
#include "ECS/entity/Entity.hpp"
#include "ECS/registry/Registry.hpp"
#include "plugins/components/health/Health.hpp"
#include "ECS/utilities/SparseArray.hpp"
#include "DLLoader/DLLoader.hpp"

void redirect_all_stdout(void) {
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Test for registering and adding a Health component to an entity
Test(ComponentManager, register_and_add_health_component)
{
    ECS::Registry reg;
    std::string healthPluginPath = "../../bin/components/Health.so";

    bool exception_thrown = false;

    try {
        // Register Health component
        reg.componentManager().registerComponent<Components::Health>(healthPluginPath);

        // Create an entity
        ECS::Entity entity = reg.entityManager().spawnEntity();

        // Load and add a Health component to the entity
        std::unique_ptr<Components::Health> healthComponent = reg.componentManager().loadComponent<Components::Health>(true);
        reg.componentManager().addComponent<Components::Health>(entity, std::move(healthComponent));
    } catch (const std::exception &e) {
        exception_thrown = true;
    }

    // Ensure no exception was thrown
    cr_assert_not(exception_thrown, "An exception was thrown when registering or adding a component.");
}

// Test for adding a Health component and checking its value
Test(ComponentManager, add_health_component)
{
    ECS::Registry reg;
    std::string healthPluginPath = "../../bin/components/Health.so";

    try {
        // Register the Health component
        reg.componentManager().registerComponent<Components::Health>(healthPluginPath);

        // Create an entity and add a Health component to it
        ECS::Entity entity = reg.entityManager().spawnEntity();
        std::unique_ptr<Components::Health> healthComponent = reg.componentManager().loadComponent<Components::Health>(true);
        reg.componentManager().addComponent<Components::Health>(entity, std::move(healthComponent));

        // Get the SparseArray for Health components
        SparseArray<Components::Health> &healthComponents = reg.componentManager().getComponents<Components::Health>();

        // Check if the component was added to the entity
        cr_assert_not_null(healthComponents[entity], "Health component was not added to the entity.");
        cr_assert_eq(healthComponents[entity]->currentHealth, 100, "Initial health value is incorrect.");  // 100 is the default value
        cr_assert_eq(healthComponents[entity]->maxHealth, 100, "Max health value is incorrect.");          // 100 is the default value
    } catch (const std::runtime_error &e) {
        cr_assert_fail("Unexpected runtime error: %s", e.what());
    }
}

// Test for removing a Health component from an entity
Test(ComponentManager, remove_health_component)
{
    ECS::Registry reg;
    std::string healthPluginPath = "../../bin/components/Health.so";

    try {
        // Register the Health component
        reg.componentManager().registerComponent<Components::Health>(healthPluginPath);

        // Create an entity and add a Health component to it
        ECS::Entity entity = reg.entityManager().spawnEntity();
        std::unique_ptr<Components::Health> healthComponent = reg.componentManager().loadComponent<Components::Health>(true);
        reg.componentManager().addComponent<Components::Health>(entity, std::move(healthComponent));

        // Remove the Health component from the entity
        reg.componentManager().removeComponent<Components::Health>(entity);

        // Get the SparseArray for Health components
        SparseArray<Components::Health> &healthComponents = reg.componentManager().getComponents<Components::Health>();

        // Check if the component was removed from the entity
        cr_assert_null(healthComponents[entity], "Health component was not removed from the entity.");
    } catch (const std::runtime_error &e) {
        cr_assert_fail("Unexpected runtime error: %s", e.what());
    }
}
