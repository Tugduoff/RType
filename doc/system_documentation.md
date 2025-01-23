# Project System Plug-ins Documentation

This document provides guidelines for creating plugins for the ECS-based game engine. Plug-ins serve as the building blocks for games by defining custom components, systems, and logic.

## Table of Contents

1. [Introduction](#introduction)
2. [What are Systems ?](#what-are-systems-)
3. [The ISystem Interface](#the-isystem-interface)
4. [The ASystem Abstract Class](#the-asystem-abstract-class)
5. [Creating a System](#creating-a-system)
    - [System Header file](#system-header-file)
    - [System Implementation](#system-implementation)
6. [Exemple: Position System](#exemple-position-system)
7. [Provided Systems](#provided-systems)

## Introduction

The ECS (Entity Component System) architecture separates game logic into three distinct elements: Entities, Components, and Systems. Systems are responsible for operating on entities based on their components and implementing game logic.

This document outlines the structure of systems, their integration into the ECS engine, and provides examples to help you create custom systems for your game.

## What are Systems ?

Systems define the logic of your game. Each system is responsible for a specific functionality, such as movement, rendering, or collision detection. They interact with entities by operating on their components. Systems are the primary drivers of game logic in an ECS framework.

A system:
- Runs every frame (or at defined intervals).
- Operates on specific component types.
- Implements game logic, such as updating positions or checking collisions.

## The ISystem Interface

The ISystem interface provides the base structure for all systems. It defines two core functions that every system must implement: run and init.

ISystem Methods:
```cpp
    void run(Engine::GameEngine &engine) // Executes the system's logic during the game loop.

    void init(Engine::GameEngine &engine) // Initializes the system. Use this method to register required components and perform setup tasks.
```

Exemple: ISystem.hpp
```cpp
namespace Systems {
    class ISystem {
    public:
        virtual ~ISystem() = default;

        virtual void run(Engine::GameEngine &engine) = 0;
        virtual void init(Engine::GameEngine &engine) = 0;
    };
};
```

## The ASystem Abstract Class

The ASystem abstract class inherits from ISystem and serves as a base for implementing systems. While it does not add any new functionality, it provides a standard structure for all systems.

Exemple: ASystem.hpp
```cpp
namespace Systems {
    class ASystem : public ISystem {
    public:
        ASystem() = default;
    };
};
```

## Creating a System
### System Header File
Each system is defined in its own header file, which declares the system class, its constructors, and overrides the run and init methods.

Exemple: Move System Header
```cpp
namespace Systems {
    class MoveSystem : public ASystem {
    public:
        MoveSystem() = default;
        MoveSystem(libconfig::Setting &config);
        ~MoveSystem() = default;

        void run(Engine::GameEngine &engine) override;
        void init(Engine::GameEngine &engine) override;
    };
};
```
### System Implementation

The system logic is implemented in a .cpp file. This includes:

- Registering required components in the init function.
- Updating components in the run function based on the game logic.

Example: MoveSystem Implementation
```cpp
void Systems::MoveSystem::run(Engine::GameEngine &engine) {
    auto &reg = engine.getRegistry();
    try {
        auto &posArr = reg.componentManager().getComponents<Components::Position>();
        auto &velArr = reg.componentManager().getComponents<Components::Velocity>();

        for (auto &&[i, pos, vel] : IndexedZipper(posArr, velArr)) {
            pos.x += vel.x;
            pos.y += vel.y;
            engine.updateComponent(i, pos.getId(), pos.serialize());
        }
    } catch (std::runtime_error &e) {
        std::cerr << "Move Error: " << e.what() << std::endl;
    }
}

void Systems::MoveSystem::init(Engine::GameEngine &engine) {
    engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position");
    engine.registerComponent<Components::Velocity>("./plugins/bin/components/", "Velocity");
}
```

## Example: Move System

The MoveSystem provides functionality for updating the position of entities based on their velocity. It ensures:

- Entities move according to their velocity components.
- Entities are removed if they exceed predefined boundaries (DeathRange).

### Key Points:

- **Components Used**: Position, Velocity, DeathRange
- **Functionality**: Updates position based on velocity and applies boundary checks.

## Provided Systems

### Attack System
#### Description

The Attack System is responsible for managing the automatic attacks of entities in the game world. It handles the creation of projectiles, sound effects, and updates to entity actions. This system leverages various components, such as Position, Gun, and Sound, to facilitate attacks and ensure a seamless gameplay experience.

- **Automatic Attacks**: The system triggers attacks for entities like enemies, ensuring periodic firing based on the fireRate of their Gun component.
- **Projectile Creation**: Dynamically generates projectiles based on the attributes of the attacking entity, such as bullet velocity, damage, and sprite.
- **Sound Synchronization**: Updates and synchronizes sound effects for attack actions.
- **Action Management**: Handles entity actions like SHOOT_FORWARD and updates the corresponding state.

#### Use Case

The Attack System is crucial for games where entities perform automated attacks, such as enemies in shoot-'em-up games. It enables efficient management of attack logic while integrating various components to deliver a cohesive attack behavior.

- **Enemy Attacks**: Automatically fires projectiles from enemy entities at defined intervals based on their Gun component.
- **Projectile Handling**: Creates projectiles with specific properties, such as position, velocity, damage, and sprite, ensuring dynamic gameplay.
- **Action State Updates**: Manages the transition of entity actions (e.g., from IDLE to SHOOT_FORWARD) and resets states as needed.
- **Sound Effects**: Plays or updates sound effects associated with attacks, providing feedback to players.
- **Error Handling**: Catches runtime errors to ensure the game remains stable even if certain components are missing or improperly configured.

For example, in a shoot-'em-up game, the Attack System enables enemies to fire projectiles toward the player at regular intervals. It also ensures that each attack action triggers an appropriate sound effect and creates projectiles with the correct behavior, such as moving in a straight line or homing toward the player.

#### Workflow

1. **Initialize Components**: Registers necessary components (Gun, Position, Velocity, Sound, etc.) during system initialization.
2. **Run Logic**: Iterates through all entities with a Gun component, checking if they can fire based on their fireRate.
3. **Create Projectiles**: Spawns projectiles with the specified properties (position, velocity, damage, sprite, etc.).
4. **Update Actions**: Modifies entity actions to reflect attack states (e.g., SHOOT_FORWARD).
5. **Play Sounds**: Synchronizes sound effects associated with attack actions.
6. **Error Recovery**: Handles exceptions, such as missing components, by adding default components or skipping the entity.

This system is a key part of the game logic, allowing dynamic interactions between attacking entities and the game world

### Behavior System
#### Description

The Behavior System is responsible for managing the movement and attack patterns of non-controllable entities within the game world. It ensures dynamic behaviors, such as looping movement along axes, zigzag patterns, and attack synchronization for enemies. The system integrates various components, including Position, Velocity, DeathRange, and Gun, to deliver a seamless and engaging gameplay experience.

- **Axis Loops**: Implements looping behavior for entities along the X and Y axes, ensuring entities stay within defined boundaries.
- **Zigzag Patterns**: Creates zigzag movement patterns for entities within specified ranges.
- **Dynamic Movement**: Adjusts entity velocity dynamically based on position and boundaries.
- **Attack Synchronization**: Integrates attack patterns with movement, enabling entities to attack while moving in specific patterns.
- **Error Handling**: Ensures system stability by managing missing or improperly configured components.

#### Use Case

The Behavior System is essential for games that require dynamic, non-linear movement and attack patterns for enemy entities. It enhances gameplay by providing more engaging and varied enemy behaviors.

- **Enemy Movement**: Controls enemy movement to follow looping or zigzag patterns within defined boundaries, making enemy interactions more challenging.
- **Boundary Handling**: Ensures entities remain within their assigned DeathRange, resetting or adjusting their movement direction accordingly.
- **Attack Integration**: Synchronizes movement with attack actions for entities equipped with a Gun component.
- **Error Handling**: Recovers gracefully from missing components, ensuring the system continues to function without critical failures.

For example, in a shoot-'em-up game, the Behavior System enables enemies to move in looping or zigzag patterns while firing projectiles at the player. This dynamic movement creates an engaging and challenging experience for players.
#### Workflow

1. **Initialize Components**: Registers required components (Position, Velocity, DeathRange, Gun, etc.) during system initialization.
2. **Axis Loops**: Implements movement loops along the X or Y axis for entities within specified boundaries, adjusting their velocity dynamically.
3. **Zigzag Movement**: Creates zigzag movement patterns along the X or Y axis for entities, factoring in their position and velocity.
4. **Attack Synchronization**: Coordinates movement with attack actions for entities with a Gun component.
5. **Update Components**: Updates the state of Position, Velocity, and other components to reflect movement and attack changes.
6. **Error Recovery**: Handles runtime errors, such as missing components, by skipping affected entities or initializing default values.

By enabling dynamic and varied behaviors, the Behavior System ensures that enemy interactions are challenging and contribute to an engaging gameplay experience.

### Collision System
#### Description

The Collision System is responsible for detecting and managing interactions between entities within the game world. This includes handling collisions between player projectiles and enemies, enemy projectiles and the player, and any other interactions requiring collision detection. It ensures proper responses, such as applying damage, triggering death effects, and updating relevant components. The system integrates Position, Collider, Type, Damage, Health, and other components to facilitate efficient collision management.

- **Collision Detection**: Identifies and processes collisions between entities, such as projectiles and targets.
- **Damage Application**: Reduces the health of entities upon collision and eliminates entities when their health reaches zero.
- **Death Effects**: Spawns visual or sound effects when entities are destroyed.
- **Cooldown Management**: Handles cooldown periods for effects triggered by collisions.
- **Error Handling**: Ensures system stability by managing missing or improperly configured components.

#### Use Case

The Collision System is crucial for any game involving interactions between entities, such as projectiles hitting enemies or the player. It enhances gameplay by ensuring realistic and timely responses to collisions, contributing to an engaging and dynamic game environment.

- **Projectile-Enemy Interaction**: Detects collisions between player projectiles and enemies, applies damage, and triggers death effects when applicable.
- **Projectile-Player Interaction**: Manages collisions between enemy projectiles and the player, reducing player health accordingly.
- **Effect Handling**: Spawns visual or auditory effects when entities are destroyed, enriching the game experience.
- **Error Recovery**: Ensures that missing components or errors during collision checks do not cause system failures.

For example, in a shoot-'em-up game, the Collision System detects when a player's projectile hits an enemy, reduces the enemy's health, and destroys the enemy if its health reaches zero. Simultaneously, it spawns a death animation or sound effect, adding to the immersive experience.

#### Workflow

1. **Initialize Components**: Registers necessary components (Position, Collider, Type, Damage, Health, etc.) during system initialization.
2. **Check Player Projectile Collisions**: Identifies projectiles belonging to the player and checks for collisions with enemy entities. Applies damage to enemies, triggers death effects, and removes projectiles after impact.
3. **Check Enemy Projectile Collisions**: Detects collisions between enemy projectiles and the player. Reduces player health and removes projectiles upon impact.
4. **Spawn Death Effects**: Handles the creation of death effects, such as animations or sounds, for destroyed entities.
5. **Cooldown Management**: Monitors and updates cooldown periods for spawned effects to ensure proper timing and removal.
6. **Error Recovery**: Handles exceptions and skips affected entities to maintain system stability.

By providing robust collision detection and handling, the Collision System ensures seamless interactions between entities, enhancing the gameplay experience while maintaining system performance and reliability.

### Config System
#### Description

The Config System is responsible for loading and managing the game’s configuration data. This includes parsing configuration files, extracting entity and component templates, and applying these templates to the game world. It ensures entities are instantiated with the appropriate components and configurations, facilitating dynamic game world creation based on external configuration files.

- **Configuration Loading**: Reads configuration files and loads game data, such as entity templates and their components.
- **Entity Template Management**: Manages templates for entities, ensuring they are correctly instantiated with their required components.
- **Entity Initialization**: Instantiates entities based on templates and configuration data, adding the necessary components to the entities.
- **Sound Component Initialization**: Ensures the correct initialization and activation of sound components when entities are spawned.
- **Error Handling**: Manages missing or improperly defined components in entities, ensuring system stability by skipping over erroneous configurations.
- **Cooldown Management**: Handles the timing of spawning entities based on their defined spawnTime.

#### Use Case

The Config System is critical for any game where dynamic world generation or configuration-based gameplay is required. It allows for a flexible game design by decoupling entity creation and configuration from hardcoded logic. This is particularly useful for managing large numbers of entities or game configurations that may change over time, such as in a moddable game or a game with procedurally generated content.

- **Dynamic Entity Creation**: Allows for entities to be created based on configuration data without needing to hardcode each one.
- **Game Configuration Loading**: Ensures that all necessary game configurations, including entity templates and components, are loaded at runtime.
- **Error Recovery**: Ensures missing or invalid components in the configuration files do not crash the game by skipping over invalid data.

For example, in a shoot-'em-up game, the Config System would load a template for an enemy type, and based on the configuration data, spawn the appropriate enemies at the right times, each with the required components like Position, Health, and Velocity.

#### Workflow

1. **Load Configuration File**: Reads the configuration file containing game data such as entity templates, components, and spawn timings.
2. **Parse Configuration Data**: Extracts data for entities, including their types, components, and arguments.
3. **Create Entity Templates**: Defines templates for entities, specifying the components required and their configurations.
4. **Instantiate Entities**: Spawns entities based on the loaded templates, applying components and configurations to the new entities.
5. **Handle Sound Components**: Initializes and activates sound components for entities that require them, such as ambient or spawn sounds.
6. **Cooldown Management**: Ensures entities are spawned at the correct times by checking the spawnTime against the elapsed time.
7. **Error Recovery**: Skips over any entities or components that are missing or incorrectly defined, ensuring the game continues running without crashing.

By separating the game’s configuration from its code, the Config System facilitates dynamic content generation and easy updates to the game world, supporting a more modular and flexible game design.

### Input System
#### Description

The Input System is responsible for managing user inputs and translating them into corresponding actions within the game world. This system handles player movement (based on user inputs like forward, backward, left, and right), and triggers actions such as shooting projectiles. It integrates several components like Controllable, Acceleration, Velocity, Gun, and Sound to ensure smooth interaction between user inputs and the game environment.

- **Movement Handling**: Processes inputs for player movement (forward, backward, left, right), adjusting velocity and acceleration.
- **Action Triggering**: Handles various actions such as shooting, using cooldowns to prevent spamming.
- **Projectile Creation**: Instantiates projectiles when the player triggers a shooting action.
- **Cooldown Management**: Ensures shooting actions respect the weapon cooldown, preventing multiple shots in a short period.
- **Sound Effects**: Plays associated sound effects when actions are triggered, like shooting.

#### Use Case

The Input System is vital for controlling player entities in games with interactive elements like movement and combat. It allows players to control characters through intuitive actions like moving and shooting, contributing significantly to gameplay immersion.

- **Movement Input Handling**: Detects directional inputs (forward, backward, left, right) and adjusts the entity's velocity accordingly.
- **Shooting Action**: Creates projectiles when the player triggers the shoot action, handling weapon cooldowns and playing sound effects.
- **Error Recovery**: Ensures that invalid or missing components do not cause system failures during input processing.

For example, in a shoot-'em-up game, the Input System allows the player to move their character using the arrow keys or joystick, and shoot projectiles at enemies when pressing a designated button.

#### Workflow

1. **Initialize Components**: Registers the necessary components such as Acceleration, Controllable, Velocity, Gun, Damage, Position, and Collider to ensure the Input System has all the data it needs to manage player inputs.
2. **Movement Input Processing**: Checks if movement inputs (forward, backward, left, right) are triggered and adjusts the velocity of the entity accordingly by considering its acceleration.
3. **Action Input Processing**: Processes any action triggers (such as shooting) and performs the corresponding action (e.g., creating a projectile).
4. **Projectile Creation**: Instantiates a new projectile when the player shoots, initializing its position, velocity, collider, damage, and type. It also manages weapon cooldowns to prevent continuous shooting.
5. **Sound Handling**: Plays the appropriate sound effects (e.g., an "ATTACK" sound) when an action like shooting occurs, while managing sound state to ensure it's played at the correct times.
6. **Cooldown Management**: Ensures that shooting actions adhere to cooldown limits by checking the gun's elapsed time before allowing another shot.
7. **Error Handling**: Catches exceptions and logs errors (e.g., missing components), ensuring the system remains stable and responsive.

The Input System manages the translation of user input into in-game actions, ensuring smooth movement, interaction, and combat mechanics, and enhancing the player's experience.

### Move System
#### Description

The Move System is responsible for updating the positions of entities based on their velocities. It modifies the Position and Velocity components to move entities accordingly, and checks if entities have moved out of a defined range, triggering their removal when they exceed the bounds. It supports the diminishing velocity feature, where the velocity decreases over time as specified by a diminishing factor. The system also integrates the DeathRange component to ensure entities are removed if they move beyond their designated area.

- **Position Update**: Modifies the position of entities based on their velocity.
- **Velocity Diminishing**: Reduces the velocity over time if a diminishing factor is provided.
- **Entity Removal**: Removes entities that exceed the boundaries defined in the DeathRange component.
- **Error Handling**: Catches exceptions related to missing or misconfigured components, ensuring smooth operation.

#### Use Case

The Move System is essential for any game that requires dynamic movement of entities. It handles updating the position of entities, such as players or projectiles, based on their velocity, and removes entities that exceed predefined boundaries, ensuring they don't exist outside the game world.

- **Player Movement**: Moves the player entity based on its velocity, with adjustments based on any diminishing velocity.
- **Projectile Movement**: Moves projectiles according to their velocity, handling both movement and removal when they leave the valid game area.
- **Boundary Check**: Monitors entities' positions against the defined boundaries in the DeathRange component and removes entities that exceed the bounds.
- **Diminishing Velocity**: Gradually reduces velocity for entities that have a diminishing factor, simulating deceleration or friction effects.

For example, in a space shooter game, the Move System updates the position of a player's ship based on its velocity and slows down the ship as the diminishing factor of the velocity decreases. It also ensures that the ship is removed when it flies off the edges of the screen, contributing to a clean and smooth gameplay experience.

#### Workflow

1. **Initialize Components**: Registers necessary components (Position, Velocity, DeathRange) during system initialization.
2. **Update Positions**: Iterates through entities with Position and Velocity components, updating their positions by adding the velocity to the current position.
3. **Handle Diminishing Velocity**: Reduces the velocity based on the diminishing factor, simulating friction or other deceleration effects.
4. **Update Components**: Updates the Position and Velocity components of each entity based on the calculated changes.
5. **Boundary Check**: Compares the position of entities with the limits defined in the DeathRange component. If the entity exceeds the bounds, it is removed from the game world.
6. **Error Handling**: Catches and reports any exceptions (such as missing or invalid components) to prevent crashes and ensure system stability.

By continuously updating positions and ensuring entities remain within valid bounds, the Move System maintains the movement mechanics within the game and prevents entities from "floating away" beyond the game world.

### SFML System

## Version

R-Typeuh system plug-ins documentation version 0.1.0, 23/01/2024, written by Tibaud Le Maout