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
6. [Example: Position System](#example-position-system)
7. [Provided Systems](#provided-systems)
    - [Attack System](#attack-system)
    - [Behavior System](#behavior-system)
    - [Collision System](#collision-system)
    - [Config System](#config-system)
    - [Input System](#input-system)
    - [Move System](#move-system)
    - [Action Manager System](#action-manager-system)
    - [Display System](#display-system)
    - [Input System (SFML)](#input-system-1)
    - [SFML System](#sfml-system-1)
    - [Shader System](#shader-system)
    - [Sound System](#sound-system)
    - [Sprite Component System](#sprite-component-system)
    - [Text System](#text-system)
    - [Texture System](#texture-system)

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

Example: ISystem.hpp
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

Example: ASystem.hpp
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

Example: Move System Header
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

## SFML System

### Action Manager System
#### Description

The Action Manager System is responsible for managing and updating the actions of entities in the game world. It processes the ActionComponent and SpriteComponent associated with each entity, ensuring that the correct action is applied and the appropriate sprite is displayed. The system handles action transitions, texture updates, and component registration to facilitate smooth and dynamic changes in entity behavior.

- **Action Processing**: Updates the action of an entity based on its current and next action states.
- **Sprite Updates**: Ensures the correct texture is loaded and displayed for each entity based on its action.
- **Component Management**: Registers and manages the Action and Sprite components for each entity.
- **Error Handling**: Ensures that missing or invalid components are handled gracefully without affecting the system's stability.

#### Use Case

The Action Manager System is critical for games with dynamic entity behaviors, such as player and enemy actions. It ensures that entities' actions are properly reflected in their visual representation by updating their sprites according to the actions they perform. This system is essential for providing visual feedback to players, such as showing an entity in different states (idle, running, attacking) as they perform various actions.

- **Action-Triggered Sprite Updates**: Changes the sprite of an entity based on its current and next action (e.g., idle to running, attacking).
- **Action Component Registration**: Ensures that entities have an ActionComponent to define their behavior and action states.
- **Sprite Component Registration**: Ensures that entities have a SpriteComponent to display the appropriate texture for their action.

For example, in a character-driven game, the Action Manager ensures that when a character transitions from walking to attacking, their sprite is updated accordingly to reflect the change in action.

#### Workflow

1. **Initialize Components**: Registers the necessary components (ActionComponent, SpriteComponent) during system initialization.
2. **Action Processing**: Iterates through entities that have both ActionComponent and SpriteComponent and processes their actions. It compares the current and next actions to determine if the sprite needs to be updated.
3. **Texture Loading**: Based on the action specified in the ActionComponent, the system loads the appropriate texture for the sprite using the loadTextureForAction function.
4. **Component Addition**: If an entity is missing an ActionComponent, the system automatically creates and adds it.
5. **Error Handling**: Catches exceptions related to missing or invalid components, ensuring the system continues running smoothly even if some entities are not properly configured.

By managing entity actions and ensuring that the correct sprite is displayed for each action, the Action Manager System contributes to a dynamic and visually engaging gameplay experience.

### Display System
#### Description

The Display System is responsible for rendering entities in the game world by drawing their corresponding sprites and text to the screen. It handles the management of texture loading, sprite positioning, and applying shaders to ensure that the game world is visually represented. The system ensures that each entity’s sprite and text are rendered correctly according to its position, scale, and layer.

- **Texture Loading**: Loads static, animated, and action-based textures for entities from configuration files.
- **Sprite Rendering**: Draws sprites to the window based on the position and scale components of entities.
- **Layer Management**: Renders entities on different layers to create proper stacking and ordering in the visual scene.
- **Text Rendering**: Displays text entities in the correct position on the screen.

#### Use Case

The Display System is vital in any game where entities need to be visually represented on the screen. It is essential for games with 2D or 3D elements, where sprites, textures, and text need to be dynamically updated and rendered in a game loop.

- **Rendering Entities**: Ensures that all entities with the appropriate components are drawn to the screen in their correct positions and with the correct textures.
- **Dynamic Texture Changes**: Handles the loading and updating of textures based on the entity’s state (static, animated, or action-based).
- **Layered Rendering**: Renders entities in layers, allowing for correct visual stacking (e.g., characters in the foreground or background).
- **Shader Application**: Uses shaders to apply visual effects to sprites (e.g., lighting or effects).

For example, in a platformer game, this system ensures that characters, obstacles, and background elements are drawn in the right order, with the right texture, and with any applied shaders.

#### Workflow

1. **Component Registration**: Registers necessary components like Position, SpriteID, Scale, and Text components to the game engine. This allows entities to have their relevant visual components initialized and tracked by the system.
2. **Configuration Loading**: Loads texture and shader configurations from files. Textures can be static, animated, or action-based, and shaders are applied for visual effects.
3. **Rendering Loop**: During the game loop, the system iterates through entities, checks for their respective components, and renders them on the screen based on their position and layer.
4. **Texture Assignment**: If an entity is missing a sprite component, the system automatically creates and assigns the necessary texture(s) based on the entity’s SpriteID and associated actions.
5. **Error Handling**: Handles errors gracefully, such as missing textures or components, by ensuring the system continues running without crashing.

By managing textures, sprite positioning, and scale, the Display system provides an essential function for rendering and displaying entities in a visually dynamic and interactive game environment.

### Input System
#### Description

The Input System handles all user inputs from the keyboard or mouse and updates the corresponding Controllable and ActionComponent for each entity. It ensures that actions are triggered based on user input (such as movement and actions like shooting) and that these inputs are correctly mapped to entity actions. This system also manages the registration and updating of key bindings, processes input events, and determines the appropriate actions based on user interaction.

- **Action Handling**: Processes and determines actions based on user input, including movement and shooting.
- **Component Registration**: Ensures that the necessary components (Controllable and ActionComponent) are properly registered for entities that require input handling.
- **Key Binding Management**: Maps user inputs (keyboard or mouse events) to specific actions and updates entity components accordingly.
- **Input Event Processing**: Handles input events such as key presses and releases, translating them into appropriate entity actions.

#### Use Case

The Input System is essential for player-driven input in real-time games, where entities perform actions based on user commands. It facilitates movement and action mappings (e.g., forward, backward, left, right, shoot) and ensures that the correct actions are triggered based on input events. This system is particularly useful in games where user control is central to gameplay, such as in character or vehicle movement and combat systems.

- **Dynamic Action Mapping**: Ensures that user inputs like key presses or mouse clicks trigger the correct entity actions.
- **Controllable Component Registration**: Ensures that all entities requiring input handling have the Controllable component attached.
- **Action Component Registration**: Ensures that all entities with user-controlled actions have an ActionComponent attached, representing their current action state.

For example, in a shooter game, the Input System would handle the mapping of keys for movement (e.g., WASD) and actions (e.g., mouse clicks for shooting) to the entity's action states, ensuring that the player's commands are executed in-game.

#### Workflow

1. **Component Initialization**: Registers Controllable and ActionComponent during system initialization to ensure that entities have the necessary components for input handling.
2. **Controllable Update**: Iterates over all entities with the Controllable component and updates their key bindings to the default values (e.g., WASD for movement, mouse buttons for shooting).
3. **Input Event Processing**: Polls for input events from the user (key presses, mouse clicks) and processes these events by calling the handleInput function.
4. **Action Determination**: Based on the current input states (e.g., forward, backward, left, right), the system determines the appropriate action for each entity and updates the ActionComponent accordingly.
5. **Action Execution**: Executes the determined action (e.g., move forward, shoot) and updates the entity’s state or visual representation in the game world.
6. **Error Handling**: Catches any errors related to invalid or missing components and ensures the system operates smoothly without crashes.

By processing user inputs and mapping them to entity actions, the Input System is crucial for interactive gameplay. It ensures that player commands are executed correctly, facilitating responsive and dynamic in-game behavior.

### SFML System
#### Description

The SFML System is responsible for managing the game window, input, display, and sound, all through the SFML library. It integrates the various subsystems such as the Display System, Input System, Action Manager, Menu, and Sound System, ensuring that the game loop operates smoothly and efficiently. This system is the backbone of the game's interaction with the user and the display, offering the necessary components for rendering, handling user inputs, and managing sounds.

- **Window Management**: Handles the creation and management of the SFML window.
- **Input Handling**: Processes player inputs such as keyboard, mouse, or controller inputs.
- **Display Management**: Manages the rendering of graphical elements using the SFML display system.
- **Sound Management**: Integrates the sound system to handle in-game sounds and music.
- **Component Interaction**: Interacts with the other subsystems (like Action Manager, Menu) and the various components of the game engine to ensure smooth gameplay.

#### Use Case

The SFML System is crucial for games that need to manage visual and audio output as well as player input in real-time. It ensures that all subsystems are executed in the correct order and that the game maintains synchronization between the various components such as the window, input, display, and sound.

- **Real-time Game Interaction**: Ensures that the player’s inputs are processed and that the appropriate graphical and audio feedback is provided.
- **Multi-component Management**: Coordinates with the Input System, Display System, Menu, Action Manager, and Sound System to ensure they all work together seamlessly.

For instance, in a real-time game where the player moves a character on screen, the SFML System handles the user’s input, updates the character’s position, and ensures the corresponding sprite and sound are displayed and played, respectively.

#### Workflow

1. **Initialization**:Initializes the subsystems (Display, Input, Action Manager, Menu, Sound) using configuration settings. Creates the SFML window with the given width, height, and title. Configures the initial view of the window for rendering.
2. **Game Loop**:In each iteration of the game loop**: the Display System handles rendering the game scene on the window. The Input System processes player inputs, such as keyboard and mouse events. The Action Manager processes game actions, ensuring entities perform the correct actions and update their visual representation. The Menu system ensures that the main menu or in-game menu is properly displayed and interacted with. The Sound System manages audio playback for game sounds and music.
3. **Window Handling**: Ensures that the window remains open and processes events correctly.Handles window closure and sends an exit signal when the window is closed.
4. **Component Registration**: Automatically registers the necessary subsystems like Display, Input, and Sound, ensuring that they are properly initialized and can be accessed during the game loop.
5. **Error Handling**:Catches any exceptions that may arise from creating subsystems like the Display system and handles them gracefully by outputting an error message and halting the application.

By integrating all essential subsystems into a cohesive framework, the SFML System ensures that the game can be played interactively and visually, with smooth performance and immediate feedback to the player.

### Shader System
#### Description

The Shader System is responsible for managing and loading shaders used for rendering effects in the game. It handles the loading of shader programs, including both vertex and fragment shaders, and ensures that the shaders are correctly applied to the graphical elements of the game. This system is crucial for implementing advanced graphical effects such as lighting, color manipulation, or custom rendering effects, adding depth and visual quality to the game world.

- **Shader Loading**: Loads shader files (vertex, fragment) and ensures they are compiled and linked correctly.
- **Shader Application**: Applies shaders to entities or game elements for rendering custom effects.
- **Error Handling**: Catches errors related to shader loading and compilation, ensuring the game continues running even if some shaders fail to load.

#### Use Case

The Shader System is essential in games where custom visual effects or complex rendering techniques are needed. It allows for dynamic, real-time effects that can be applied to entities and the game world, such as lighting, water effects, or special visual effects like post-processing filters.

- **Custom Shader Effects**: Enables the game to apply custom shaders to entities or backgrounds, enhancing the visual experience.
- **Dynamic Shader Application**: Allows different shaders to be applied dynamically to various parts of the game world, such as applying a different shader to the player or enemies when they enter special states.

For example, in a game with a dynamic lighting system, the Shader System would apply a shader to entities and environments to create lighting effects, shadowing, or glow effects based on the current game state.

#### Workflow

1. **Initialization**: The system initializes by loading shader settings from a configuration file. This includes the shader's name and file path. It loads the shader file from the specified path and compiles it into a usable shader program.
2. **Shader Loading**: The system loads shaders from disk, using the specified path. It checks for the presence of required attributes like name and path in the configuration file.
3. **Shader Application**: Once shaders are loaded, they are ready to be applied to graphical elements within the game. This may involve attaching shaders to specific entities, backgrounds, or scene layers.
4. **Error Handling**: If the shader file is missing or corrupted, or if the shader fails to compile, the system catches the error and displays an appropriate message, preventing the game from crashing and allowing it to proceed with default rendering.

The Shader System allows developers to create  visual effects in the game in the hope of making it accessible for all.

### Sound System
#### Description

The Sound System is responsible for managing and playing sounds in the game, ensuring that audio effects are triggered appropriately during gameplay. It interfaces with sound components to load, play, and reset sounds based on the current game state. This system also ensures that sound effects are properly registered and handled for each entity that requires audio feedback, providing a crucial element for player immersion through sound effects such as footsteps, weapon fire, or background music.

- **Sound Management**: Loads, plays, and resets sounds based on the current game state.
- **Component Integration**: Ensures that entities with sound components are properly registered and updated to reflect the current audio requirements.
- **Sound Playback**: Handles the playback of sound instances, including starting, stopping, or resetting sounds as needed.
- **Error Handling**: Catches any errors during sound component processing and ensures that the game continues without crashing.

#### Use Case

The Sound System is essential in games where audio feedback is critical for player immersion and game experience. It supports a variety of sound-related functionalities, including background music, sound effects for actions (e.g., shooting or jumping), and ambient sounds. The system allows sounds to be dynamically applied to entities and game events, making the game feel alive and responsive.

- **Dynamic Sound Playback**: Ensures that sounds are played or reset dynamically based on the game’s context, such as triggering sound effects when certain events occur.
- **Sound Component Management**: Allows entities to have unique sound settings (e.g., specific sounds for actions or conditions) and ensures the correct sound components are updated.

For example, in a shooter game, the Sound System would play sounds for actions like shooting, jumping, or taking damage. It would ensure that sounds are played at the right times, with the correct parameters, such as volume or pitch.
Workflow

1. **Initialization**: The system initializes by registering the necessary sound components with the game engine. This includes registering sound-related components such as Sound and SfmlSoundComponent.

2. **Sound Registration**: The Sound components, which define the sound data (e.g., sound file, volume, etc.), are loaded from the engine or configuration files.

3. **Sound Playback**: During the game loop, the system iterates through all sound components and plays the appropriate sounds.Sounds are either played or reset based on the values associated with each sound instance. If necessary, new SfmlSoundComponent instances are created and added to entities that require sound.

4. **Error Handling**: If a sound component fails to be processed (e.g., missing or invalid sound data), the system catches the exception and ensures that the entity still has a valid sound component attached. In cases where an entity doesn’t have a sound component, a new one is created and registered.

5. **Sound Component Update**: Once the sound is played or reset, the sound component is updated accordingly, ensuring it reflects the current state of the sound instance (e.g., whether it's playing or paused).

The Sound System ensures that all necessary sound effects and components are registered and updated correctly throughout the game, adding audio feedback to enhance the player's experience.

### Sprite Component System
#### Description

The Sprite Component System is responsible for managing and updating sprite-based visuals for game entities, ensuring that the correct textures are applied and animated according to the current action or state of the entity. This system interacts with textures, stores them, and applies them to the entity’s sprite. The system enables dynamic texture switching, allowing entities to change their appearance based on their current action (e.g., idle, moving, attacking). The component also supports animation by cycling through a sequence of textures over time.

- **Texture Management**: Manages multiple textures and selects the correct one based on the current action of the entity.
- **Texture Application**: Applies the selected texture to the corresponding sprite, ensuring that it reflects the correct visual state.
- **Animation Handling**: Handles the updating and switching of textures for animated actions, such as walking or attacking.
- **Action Control**: Supports stopping the current action or transitioning to a new one, allowing entities to change their behavior and appearance dynamically.
- **Error Handling**: Provides basic error handling for missing textures or invalid states, ensuring that the sprite system continues running without issues.

#### Use Case

The Sprite Component System is essential for any game that involves visual feedback through character or object animations. It is used in scenarios where an entity’s visual state needs to change dynamically, such as a character switching from idle to running, or an enemy transitioning between different attack phases. The system ensures that the appropriate textures are loaded and applied to entities based on their actions, supporting complex visual effects and gameplay dynamics.

- **Dynamic Texture Switching**: Allows entities to switch between multiple textures based on the current action or state, enabling smooth transitions between animations.
- **Animation Handling**: Ensures that animations are updated in real time, allowing for fluid character movement and other dynamic visual effects.
- **Action Control**: Supports switching between actions (e.g., idle, running, attacking) and ensures that the correct texture is applied for each action.

For example, in a platformer game, the Sprite Component System would manage the sprite of a character and ensure that the character's appearance changes from idle to walking, jumping, or attacking, based on player input or in-game events.

#### Workflow

1. **Initialization**: The system initializes by loading a set of textures for each entity. These textures represent various actions or states of the entity, such as idle, moving, or attacking. The first texture is applied to the sprite when no action is set.
2. **Texture Registration**: The system registers the textures corresponding to different actions for the entity. Each texture is associated with an action (e.g., "run", "jump", "attack").
3. **Action Management**: As the game progresses, the system monitors the entity's current action and checks whether the texture should be updated. If the current action finishes, the system transitions to the next action (e.g., from running to idle).
4. **Texture Switching**: If the current action requires a different texture, the system loads the texture for that action and applies it to the sprite. The system ensures that the entity transitions smoothly from one texture to another.
5. **Animation Update**: The system periodically updates the current texture’s animation, if applicable, and ensures that the sprite displays the correct frame of the animation.
6. **Error Handling**: If a texture is missing or invalid, the system catches the error and logs an appropriate message, ensuring that the game continues running with a fallback texture or action.

The Sprite Component System ensures that the visuals of entities are correctly managed and updated, making it crucial for dynamic and engaging game graphics.

### Text System
#### Description

The Text System is responsible for managing and rendering text-based content for game entities, ensuring that the text displayed is dynamic and reflective of the game’s state. It interfaces with Text components to load and render text, allowing for real-time updates to text properties like content, size, color, and position. This system is critical for user interface elements, messages, scores, or any other textual information that needs to be presented to the player during gameplay.

- **Text Management**: Loads, updates, and displays text on the screen, reflecting changes in the game state or user input.
- **Font Handling**: Ensures that the correct font is loaded and applied to text elements.
- **Text Styling**: Allows for dynamic changes to text size, color, and content, providing flexibility in text presentation.
- **Positioning**: Controls the positioning of text elements on the screen to ensure they are placed correctly in relation to other UI components or game entities.
- **Error Handling**: Provides error handling in case the font cannot be loaded or if other text-related issues arise during rendering.

#### Use Case

The Text System is essential for displaying dynamic textual information to the player. It is used for various purposes, including displaying scores, health points, instructions, dialogue, or any real-time text updates that occur during gameplay. The system ensures that text elements are correctly rendered in response to game events or user interaction.

- **Dynamic Text Updates**: Allows for real-time updates to the text displayed, such as displaying scores, health, or other player-related information that changes during the game.
- **Customizable Appearance**: Enables customization of text properties, including size, color, and position, to match the game's theme or the UI layout.
- **Rendering of In-Game Text**: Ensures that all in-game text is rendered properly, including updates based on game events or player actions.

For example, in a game where the score needs to be displayed at the top of the screen, the Text System would ensure that the score is dynamically updated and rendered with the appropriate font, size, and color.
Workflow

1. **Initialization**: The system initializes by loading the font used for rendering text. If the font cannot be loaded, an error message is logged, and a fallback font may be used.
2. **Text Registration**: The system registers Text components, which define the text content, size, color, and position. Each text component is associated with a specific position on the screen or relative to an entity.
3. **Text Update**: As the game progresses, the system checks for changes in the text properties (e.g., score updates, health changes, or new messages) and updates the text content accordingly.
4. **Text Rendering**: During the game loop, the system renders the text on the screen, using the properties set for each text component (font, size, color, and position).
5. **Error Handling**: If any issues arise (e.g., missing font file or invalid text properties), the system logs an error and attempts to continue rendering text with default settings or fallback options.

The Text System ensures that all textual information in the game is displayed properly and updated dynamically, contributing to the user interface and player experience.

### Texture System
#### Description

The Texture System is responsible for managing and updating textures for game entities, enabling the dynamic application of textures to sprites and handling texture animations. It allows for the switching of textures based on the entity’s current action or state and supports frame-based animation within textures. The system also manages texture loading, updates texture properties (e.g., position, size), and handles animation loops or resets, ensuring a smooth visual experience in the game.

- **Texture Management**: Loads and applies textures to entities, ensuring the correct texture is used based on the current action or state.
- **Animation Handling**: Controls frame-based animation within textures, cycling through texture frames at a specified update rate.
- **Texture Properties**: Manages texture properties such as position, size, and origin within the sprite, updating them dynamically as needed.
- **Action Control**: Allows for transitioning between different textures or animations based on the entity’s actions, such as switching between idle, moving, or attacking.
- **Error Handling**: Provides basic error handling for missing or incorrectly loaded textures, ensuring the game can continue even if a texture fails to load.

#### Use Case

The Texture System is essential for rendering animations and dynamic textures in games. It is used for entities that require animated textures (e.g., characters with walking or attacking animations) or for changing an entity’s texture based on its state (e.g., switching from idle to running). This system ensures that textures are loaded and updated correctly, whether for static images or animated sequences.

- **Dynamic Texture Switching**: Allows entities to dynamically change their textures based on the current action or state.
- **Frame-Based Animation**: Supports animating textures by cycling through different frames, creating smooth animations for actions like running, jumping, or attacking.
- **Action Control**: Manages texture updates to reflect changes in the entity’s action or behavior (e.g., switching from idle to moving).
- **Animation Reset**: Handles texture resets or stopping of animations based on predefined conditions (e.g., animation completion).

For example, in a game where a character moves and attacks, the Texture System would be responsible for switching between idle, moving, and attacking animations by cycling through different frames of the corresponding texture.
Workflow

1. **Initialization**: The system begins by loading the texture from the specified path. If the texture cannot be loaded, an error message is logged, and the game continues without the texture or with a fallback.
2. **Texture Registration**: The system registers the texture with the required properties, such as update rate, size, action name, and whether the animation should repeat or reset before the end.
3. **Animation Update**: The system tracks the elapsed time using a chrono object and updates the texture’s position within the sprite’s texture rectangle. It cycles through texture frames at a specified update rate.
4. **Action Handling**: As the entity’s action changes (e.g., from idle to moving), the system ensures the correct texture is used, and if animation is involved, it updates the animation frame.
5. **Texture Application**: The system applies the texture to the sprite, ensuring that it uses the correct part of the texture for the animation or static image.
6. **Error Handling**: If any issues arise with the texture, such as a missing file or incorrect dimensions, the system logs the error and either continues with a default texture or halts the animation.

The Texture System ensures that all texture-based visual effects in the game are managed and updated smoothly, providing essential visual feedback for entities' actions and animations.

## Version

R-Typeuh system plug-ins documentation version 0.2.0, 23/01/2024, written by Tibaud Le Maout