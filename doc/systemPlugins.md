# System Plugins Developer Documentation

## Purpose

This documentation is intended for developers creating **new system plugins** for the **RType** game engine. Systems define the behavior of the game by processing entities and their components at runtime. This guide explains how to extend the `ISystem` interface, initialize your system, and make it interact with components within the ECS (Entity Component System) architecture.

By following this guide, you will be able to create a new system that integrates seamlessly into the RType ECS.

---

## ISystem Interface

To add a new **system** as a plugin, you need to **inherit from the `ISystem` interface** and implement the required methods. Below is an overview of the `ISystem` interface and step-by-step instructions on how to implement it.

### Key Methods to Implement

Your custom system must define the following methods to work as a plugin within the RType engine:

### 1. `run(Engine::GameEngine &engine)`
- **Purpose**: This is the main function of the system, where the system’s logic is implemented. It is called every frame or at a defined interval.
- **Parameters**: 
  - `engine`: The reference to the `GameEngine` instance, which provides access to all entities and their components.
- **Details**:
  - This method should contain the logic of your system.
  - It interacts with components and updates them based on the system's purpose (e.g., a movement system updates position components based on velocity components).
  - Use the `engine.getRegistry()` function to access and manipulate entities and components within the ECS.

### 2. `init(Engine::GameEngine &engine)`
- **Purpose**: Initialize the system when the game engine starts.
- **Parameters**:
  - `engine`: Reference to the `GameEngine`, allowing the system to register the components it needs.
- **Details**:
  - This method is used to **register the necessary components** and perform any setup required for the system to run. 
  - For example, in a movement system, you might register the `Position` and `Velocity` components here.
  - If any errors occur during initialization, throw a `std::runtime_error`.

---

## Additional methods in the source file of the system

### 1. `entryPoint() -> std::unique_ptr<Systems::ISystem>`
- **Purpose**: This is a factory function required by the engine to dynamically load your system as a plugin. This function will return an instance of the system.
- **Args**: This function can take any args it wants, such as strings, ints and custom structs. Thoses arguments can be use to create the instance by calling a custom constructor.
- **Return Type**: `std::unique_ptr<Systems::ISystem>`
- **Details**:
  - Implement this function to **return a new instance** of your custom system.
  - The engine uses this entry point to instantiate your system at runtime.

### 2. `entryConfig() -> std::unique_ptr<Systems::ISystem>`
- **Purpose**: Similarily to `entryPoint()`, this factory function also returns an instance of the system but creates the instance using a libconfig setting.
- **Args**: This function takes a libconfig setting element that can be passed and parsed in the constructor of the instance. It allows for defining system's data (for the init function) in config files.
- **Return Type**: `std::unique_ptr<Systems::ISystem>`
- **Details**:
  - Implement this function to **return a new instance** of your custom system.
  - The engine uses this entry point to instantiate your system at runtime.

## Steps to Create a New System Plugin

1. **Create a New Class**:
   - Create a new folder in `plugins/systems/`.
   - Inside this folder, create a new `.cpp` and `.hpp` file for your system.
   - Define your system class, **inheriting from `ISystem`**.
   - Example:
     ```cpp
     class MyCustomSystem : public Systems::ISystem {
         // Implement all methods from the ISystem interface.
     };
     ```

2. **Implement the Required Methods**:
   - Ensure that your system correctly implements `run()`, `init()`, and `entryPoint()`.
   - If any of these methods are missing, the engine will not be able to use your system, and a runtime error will occur.

3. **Add Your Plugin to the Engine**:
   - Create a `CMakeLists.txt` file in your custom folder containing the following:
     ```cmake
     # Add a shared library for your system plugin
     add_library([Name of executable] SHARED
         [Name of file].cpp
     )
     ```
   - Replace `[Name of executable]` with the name you want for the system's shared library and `[Name of file]` with the name of your custom system’s `.cpp` file.
   - This will compile your system as a dynamic plugin that can be loaded by the game engine at runtime.

---

## Example: Simple Custom System Plugin

Below is an example of how you might implement a basic system plugin, **MoveSystem**, that updates the position of entities based on their velocity.

### Header File: `MoveSystem.hpp`

```cpp
/*
** EPITECH PROJECT, 2024
** RType
** File description:
** MoveSystem.hpp file
*/

#ifndef MOVESYSTEM_HPP
    #define MOVESYSTEM_HPP

    #include "systems/ISystem.hpp"
    #include "GameEngine/GameEngine.hpp"

namespace Systems {
    class MoveSystem : public ISystem {
        public:
            MoveSystem() = default;
            ~MoveSystem() = default;

            /**
             * @brief Run the system logic to update entity positions based on their velocity
             * 
             * @param engine The game engine, giving access to entities and components
             */
            void run(Engine::GameEngine &engine) override;

            /**
             * @brief Initialize the system and register the necessary components
             * 
             * @param engine The game engine
             */
            void init(Engine::GameEngine &engine) override;
    };
};

#endif // MOVESYSTEM_HPP
```

### Source File: `MoveSystem.cpp`
```cpp
/*
** EPITECH PROJECT, 2024
** RType
** File description:
** MoveSystem.cpp file
*/

#include "GameEngine/GameEngine.hpp"
#include "Move.hpp"
#include "components/position/Position.hpp"
#include "components/velocity/Velocity.hpp"
#include "library_entrypoint.hpp"
#include <iostream>
#include <stdexcept>

Systems::MoveSystem::MoveSystem(libconfig::Setting &)
{
}

void Systems::MoveSystem::run(Engine::GameEngine &engine)
{
    auto &reg = engine.getRegistry();

    try {
        auto &posComponents = reg.componentManager().getComponents<Components::Position>();
        auto &velComponents = reg.componentManager().getComponents<Components::Velocity>();

        size_t i = 0;
        for (i = 0; i < posComponents.size() || i < velComponents.size(); i++) {
            auto &pos = posComponents[i];
            auto &vel = velComponents[i];

            if (!pos || !vel)
                continue;
            pos->x += vel->x;
            pos->y += vel->y;
        }
    } catch (std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void Systems::MoveSystem::init(Engine::GameEngine &engine)
{
    if (!engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position"))
        std::cerr << "Error: Could not register Position component in system Move" << std::endl;
    if (!engine.registerComponent<Components::Velocity>("./plugins/bin/components/", "Velocity"))
        std::cerr << "Error: Could not register Velocity component in system Move" << std::endl;
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryPoint()
{
    return new Systems::MoveSystem();
}

LIBRARY_ENTRYPOINT
Systems::ISystem *entryConfig(libconfig::Setting &config)
{
    return new Systems::MoveSystem(config);
}
```

## Version

R-Type System Plugins Documentation version 0.0.1, 05/10/2024, written by Tugdual de Reviers.
