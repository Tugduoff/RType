#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/redirect.h>
#include "ECS/entity/Entity.hpp"
#include "ECS/registry/Registry.hpp"
#include "plugins/components/visible/Visible.hpp"
#include "ECS/utilities/SparseArray.hpp"
#include "DLLoader/DLLoader.hpp"

void redirect_all_stdout(void) {
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Test for registering and adding a Visible component to an entity
Test(ComponentManager, register_and_add_visible_component)
{
    ECS::Registry reg;
    std::string visiblePluginPath = "../../bin/components/Visible.so";

    bool exception_thrown = false;

    try {
        // Register Visible component
        reg.componentManager().registerComponent<Components::Visible>(visiblePluginPath);

        // Create an entity
        ECS::Entity entity = reg.entityManager().spawnEntity();

        // Load and add a Visible component to the entity
        std::unique_ptr<Components::Visible> visComponent = reg.componentManager().loadComponent<Components::Visible>(true);
        reg.componentManager().addComponent<Components::Visible>(entity, std::move(visComponent));
    } catch (const std::exception &e) {
        exception_thrown = true;
    }

    // Ensure no exception was thrown
    cr_assert_not(exception_thrown, "An exception was thrown when registering or adding a component.");
}

// Test for adding a Visible component and checking its value
Test(ComponentManager, add_visible_component)
{
    ECS::Registry reg;
    std::string visiblePluginPath = "../../bin/components/Visible.so";

    try {
        // Register the Visible component
        reg.componentManager().registerComponent<Components::Visible>(visiblePluginPath);

        // Create an entity and add a Visible component to it
        ECS::Entity entity = reg.entityManager().spawnEntity();
        std::unique_ptr<Components::Visible> visComponent = reg.componentManager().loadComponent<Components::Visible>(true);
        reg.componentManager().addComponent<Components::Visible>(entity, std::move(visComponent));

        // Get the SparseArray for Visible components
        SparseArray<Components::Visible> &visibleComponents = reg.componentManager().getComponents<Components::Visible>();

        // Check if the component was added to the entity
        cr_assert_not_null(visibleComponents[entity], "Visible component was not added to the entity.");
        cr_assert_eq(visibleComponents[entity]->isVisible, true, "Visible state is incorrect.");
    } catch (const std::runtime_error &e) {
        cr_assert_fail("Unexpected runtime error: %s", e.what());
    }
}

// Test for removing a Visible component from an entity
Test(ComponentManager, remove_visible_component)
{
    ECS::Registry reg;
    std::string visiblePluginPath = "../../bin/components/Visible.so";

    try {
        // Register the Visible component
        reg.componentManager().registerComponent<Components::Visible>(visiblePluginPath);

        // Create an entity and add a Visible component to it
        ECS::Entity entity = reg.entityManager().spawnEntity();
        std::unique_ptr<Components::Visible> visComponent = reg.componentManager().loadComponent<Components::Visible>(true);
        reg.componentManager().addComponent<Components::Visible>(entity, std::move(visComponent));

        // Remove the Visible component from the entity
        reg.componentManager().removeComponent<Components::Visible>(entity);

        // Get the SparseArray for Visible components
        SparseArray<Components::Visible> &visibleComponents = reg.componentManager().getComponents<Components::Visible>();

        // Check if the component was removed from the entity
        cr_assert_null(visibleComponents[entity], "Visible component was not removed from the entity.");
    } catch (const std::runtime_error &e) {
        cr_assert_fail("Unexpected runtime error: %s", e.what());
    }
}
