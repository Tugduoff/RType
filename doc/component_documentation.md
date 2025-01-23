# Project Component Plug-ins Documentation

This document provides guidelines for creating plugins for the ECS-based game engine. Plug-ins serve as the building blocks for games by defining custom components, systems, and logic.

## Table of Contents

1. [Introduction](#introduction)
2. [What are Components ?](#what-are-components-)
3. [The IComponent Interface](#the-icomponent-interface)
4. [The AComponent Abstract Class](#the-acomponent-abstract-class)
5. [Creating a Component](#creating-a-component)
    - [Component Header file](#component-header-file)
    - [Component Implementation](#component-implementation)
6. [Exemple: Position Component](#exemple-position-component)
7. [Provided Components](#provided-components)
    - [Acceleration Components](#acceleration-component)
    - [Action Components](#action-component)
    - [Ai Components](#ai-component)
    - [Collider Components](#collider-component)
    - [Controllable Components](#controllable-component)
    - [Damage Components](#damage-component)
    - [Death Range Components](#deathrange-component)
    - [Destruction Components](#destruction-component)
    - [Gun Components](#gun-component)
    - [Health Components](#health-component)
    - [Position Components](#position-component)
    - [Scale Components](#scale-component)
    - [Sound Components](#sound-component)
    - [Sprite Id Components](#spriteid-component)
    - [Type Components](#type-component)
    - [Velocity Components](#velocity-component)
    - [Visible Components](#visible-component)

## Introduction

Plug-ins allow developers to extend the functionality of our ECS game engine by creating reusable components. Components represent the properties or behaviors of entities, such as position, health, scale, collider and much more.

This document outlines how to create, serialize, and manage components within our game engine.

## What are Components ?

In this ECS game engine:

- **Entities**: Act as unique IDs that group components.
- **Components**: Represent data or state (e.g., Position, Velocity).
- **Systems**: Operate on entities that have a specific set of components.

Each component in the engine must inherit from the IComponent interface and, optionally, from the AComponent abstract class for convenience.

## The IComponent Interface

The IComponent interface defines the essential functions that all components must implement:

- **Serialization/Deserialization**: Convert components to/from binary data for networking or storage.
- **Size Retrieval**: Get the size of serialized data.
- **Entity Association**: Attach components to entities dynamically.
- **Casting**: Convert between specific and generic component types.

### Key Methods:
```cpp
virtual std::vector<uint8_t> serialize() = 0;  
virtual void deserialize(std::vector<uint8_t> &data) = 0;  
virtual size_t getSize() const = 0;  
virtual void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) = 0;  
virtual const std::string &getId() = 0;
```

## The AComponent Abstract Class

AComponent provides default implementations for certain methods and simplifies the development of components.

- Inherits from IComponent.
- Stores a unique string identifier (componentName).
- Implements casting utilities for SparseArray.

## Creating a Component

### Component Header File
Define the component by inheriting from AComponent or directly from IComponent.

**Template**:

```hpp
#ifndef COMPONENT_NAME_HPP
#define COMPONENT_NAME_HPP

#include "plugins/components/AComponent.hpp"

namespace Components {
    class ComponentName : public AComponent<ComponentName> {
    public:
        ComponentName(); // Default constructor
        explicit ComponentName(libconfig::Setting &config); // Config constructor
        ~ComponentName() = default;

        std::vector<uint8_t> serialize() override;
        void deserialize(std::vector<uint8_t> &data) override;
        size_t getSize() const override;
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override;

    private:
        // Component-specific data
    };
}
#endif
```

### Component Implementation

Define the behavior of your component, including serialization, deserialization, and how it is added to entities.

## Exemple: Position Component

### Position.hpp:
```hpp
#ifndef POSITION_HPP
#define POSITION_HPP

#include "plugins/components/AComponent.hpp"

namespace Components {
    class Position : public AComponent<Position> {
    public:
        Position();
        explicit Position(libconfig::Setting &config);
        Position(uint32_t x, uint32_t y, uint32_t layer);
        ~Position() = default;

        std::vector<uint8_t> serialize() override;
        void deserialize(std::vector<uint8_t> &data) override;
        size_t getSize() const override;
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override;

    private:
        uint32_t x, y, layer;
        union {
            struct { uint32_t x, y, layer; } __network;
            uint8_t __data[12];
        };
    };
}
#endif
```
### Position.cpp:
```cpp
#include "Position.hpp"

namespace Components {
    Position::Position() : AComponent("Position"), x(0), y(0), layer(0) {}
    Position::Position(libconfig::Setting &config) : AComponent("Position") { ... }
    Position::Position(uint32_t x, uint32_t y, uint32_t layer) : AComponent("Position"), x(x), y(y), layer(layer) {}

    std::vector<uint8_t> Position::serialize() { ... }
    void Position::deserialize(std::vector<uint8_t> &data) { ... }
    size_t Position::getSize() const { return sizeof(__data); }
    void Position::addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) { ... }
}
```

## Provided Components

### Acceleration Component
#### Description

The Acceleration component represents the acceleration properties of an entity, providing dynamic control of movement in multiple directions. It supports serialization and deserialization, making it suitable for single-player and multiplayer environments.

- **Acceleration Values**: Includes forward, backward, right, and left acceleration to represent directional movements.

#### Use Case

The Acceleration component is essential in games where entities have movement dynamics, such as player-controlled characters, vehicles, or projectiles. It ensures flexibility, consistency, and ease of integration for movement-related systems.

- **Dynamic Movement**: Provides directional acceleration values for smooth and controlled motion.
- **Configuration Adaptability**: Supports initialization via predefined settings or runtime parameters.
- **Network Integration**: Ensures consistent synchronization of acceleration states between the client and server in multiplayer games.

For example, in a multiplayer shoot 'em up game, the Acceleration component allows entities like spaceships or projectiles to move dynamically, enabling responsive and fluid gameplay. The ability to serialize and deserialize the acceleration values ensures consistent behavior across all players and efficient network communication.

### Action Component
#### Description

The Action component represents the current action or state of an entity, allowing dynamic control of its behavior and display. This component simplifies defining, managing, and serializing entity actions across game systems. It supports serialization and deserialization, enabling seamless integration in both single-player and multiplayer environments.

- **EntityAction Enum**: Enum values that define actions such as idle, moving, attacking, or any other predefined behaviors.

#### Use Case

The Action component is essential in games where entities have multiple states or behaviors, such as player-controlled characters, NPCs, or enemies. Common use cases include:

- **State Management**: Tracks the current state of an entity, such as idle, moving, or performing an attack.
- **Gameplay Flexibility**: Easily extends to add new states or actions for dynamic gameplay.
- **Network Integration**: Ensures consistent state synchronization for entities between the client and server in multiplayer games.

For example, in a multiplayer shoot 'em up game, the Action component can be used to manage whether an enemy is moving, firing, or waiting, ensuring that all players see consistent behavior during gameplay. The ability to serialize and deserialize actions allows for smooth state transitions and efficient network communication.

### Ai Component
#### Description

The AI component defines the behavior of an entity, allowing for configurable and reusable logic for non player controlled entities. It supports serialization and deserialization, enabling seamless integration into both local gameplay and networked environments.

- **BehaviorId**: Enum values representing predefined AI behaviors such as looping, zig-zagging, or remaining static.

#### Use Case

The AI component is crucial for games with non player controlled entities, enabling dynamic and varied gameplay experiences. Typical applications include:

- **AI Behavior Control**: Defines the movement or action pattern for NPCs, enemies, or projectiles.
- **Dynamic Gameplay**: Supports complex behavior patterns such as zig-zagging or looping movements, enhancing gameplay depth.
- **Network Synchronization**: Ensures consistent AI behaviors between clients and servers in multiplayer games.

For instance, in a shoot 'em up game, the AI component can be used to program enemies that move in predefined patterns like loops or zig-zags, providing challenges to players while maintaining performance and consistency across game sessions. With its modular design, the AI component is easily extendable, allowing developers to add new behaviors as needed.

### Collider Component
#### Description

The Collider component represents a rectangular collision box for an entity, storing the width and height of the collider. It supports serialization and deserialization for data transfer and storage, making it suitable for both local gameplay and networked environments.

- **width**: Defines the width of the rectangle used for collision detection.
- **height**: Defines the height of the rectangle used for collision detection.

#### Use Case

The Collider component is vital for games that require collision detection and resolution, including:

- **Collision Systems**: Determines overlaps and collisions between entities during gameplay.
- **Physics Simulations**: Works in tandem with velocity and position components to handle physics-based interactions.
- **Networked Gameplay**: Ensures consistent collider dimensions across client and server.

For instance, in a shoot 'em up game, the Collider component can define the hitbox of a player’s ship or enemy entities. This ensures accurate collision handling when a projectile strikes an entity or when two entities overlap. With its modular and configurable design, the component integrates seamlessly into the game engine, facilitating reliable and efficient collision detection.

### Controllable Component
#### Description

The Controllable component allows an entity to be controlled via user inputs, such as key presses. It maps specific actions to keys and provides serialization and deserialization functionality, enabling its use in both local and networked gameplay environments. The component is flexible, allowing configuration through both code and external configuration files.

- **Key Bindings**: A map linking game actions (e.g., move forward, attack) to specific keys.
- **Inputs**: Tracks the current state of movement directions (e.g., forward, backward, left, right).
- **Actions**: Tracks the current state of up to 10 actions (e.g., attack, special ability).

This component ensures consistent input handling across various game systems and facilitates seamless integration into both single-player and multiplayer contexts.

#### Use Case

The Controllable component is essential for games with player-controlled entities, enabling:

- **Character Movement**: Maps movement directions (forward, backward, left, right) to specific keys, allowing fluid control.
- **Action Triggers**: Links in-game actions like attacking or using items to user inputs.
- **Network Synchronization**: Ensures key bindings and input states are consistently transmitted and interpreted in multiplayer games.
- **Customizable Controls**: Supports configuration through external files, allowing players to customize controls.

For instance, in a shoot 'em up game, the Controllable component can be used to handle player movement and attacks. Players can move their character in any direction and activate actions like firing weapons or using power-ups. The component's modular design ensures that inputs are easily processed and communicated across different game systems.

### Damage Component
#### Description

The Damage component represents the amount of damage an entity can inflict. It is designed to store a single damage value and provides methods for serialization and deserialization, making it suitable for use in both local game logic and networked environments. The value of the damage can be used in various game mechanics, such as reducing the health of other entities or applying effects when collisions occur.

- **damage**: The amount of damage associated with the entity. It is represented as an unsigned 32-bit integer.

This component ensures that the damage data can be efficiently transmitted over a network or saved and loaded during gameplay, maintaining consistency across game states.

#### Use Case

The Damage component is essential in games where entities interact with one another, causing harm or applying effects. It is particularly useful for:

- **Combat Systems**: Calculates the damage dealt by projectiles, player attacks, or environmental hazards.
- **Collision Systems**: Determines how much damage an entity should inflict when a collision occurs.
- **Network Synchronization**: Ensures that the damage values are consistently transmitted and updated across client-server systems in multiplayer games.
- **Configurable Damage Values**: Enables easy adjustment of damage through configuration files or dynamic parameters.

For example, in a shoot 'em up game, the Damage component can be used to define the amount of damage dealt by bullets, missiles, or other projectiles. When a projectile collides with an enemy, the damage value can be used to reduce the enemy's health, triggering destruction if the health falls to zero. This modular approach ensures that the damage logic remains consistent and manageable throughout the game's development.

### Death Range Component
#### Description

The DeathRange component defines a specific region on the X and Y axes that determines when an entity should be considered out of bounds or eligible for destruction. It stores the minimum and maximum values for both the X and Y axes, allowing the game logic to determine if an entity has exceeded these bounds and should be destroyed. This component also provides methods to serialize and deserialize the data for network transmission or saving/loading game states.

- **minX**: The minimum value on the X axis, indicating the leftmost boundary of the death range.
- **maxX**: The maximum value on the X axis, indicating the rightmost boundary of the death range.
- **minY**: The minimum value on the Y axis, indicating the bottommost boundary of the death range.
- **maxY**: The maximum value on the Y axis, indicating the topmost boundary of the death range.

This component is used to determine when an entity moves outside of the defined area, which is typically used for automatically destroying out-of-bounds entities in a game.

#### Use Case

The DeathRange component is useful for entities that must be confined within a specific area or map bounds. It is used in:

- **Boundary Systems**: Determines whether an entity has crossed the defined boundaries and should be removed from the game world, such as when an enemy moves outside the screen or when a projectile exceeds the viewport.
- **Game Logic Systems**: Helps manage the gameplay by ensuring entities don't move outside of the playable area, triggering their destruction or other effects when they exceed the death range.
- **Network Synchronization**: Serializes and deserializes the component data for consistent behavior across the client-server architecture, ensuring that entities' positions and destruction logic remain synchronized.

For example, in a shoot 'em up game, the DeathRange component could be used to ensure that any enemy ships or projectiles that move beyond the screen’s borders are destroyed to avoid wasting resources on out-of-bounds entities. This helps maintain the performance and consistency of the game state across different sessions and players.

### Destruction Component
#### Description

The Destruction component handles the destruction mechanics of an entity in the game. It provides functionality to manage what happens when an entity is destroyed, such as:

- **shouldDestroy**: A boolean flag indicating whether the entity should be destroyed.
- **onDestroy**: A function or callback to execute when the entity is destroyed (e.g., spawning particles, triggering sound effects, or dropping loot).
- **isImmune**: A boolean flag indicating whether the entity is temporarily immune to destruction (e.g., during spawn protection or specific power-ups).
- **destroyAfterTime**: A timer value (in milliseconds) that determines if the entity should be destroyed after a certain amount of time, often used for temporary objects like projectiles or power-ups.

This component is crucial for managing the lifecycle of entities and integrating destruction events with the game's visual and gameplay mechanics. It supports serialization and deserialization to maintain state consistency across game sessions or networked environments.

#### Use Case

The Destruction component is essential for entities that have a finite lifespan or can be eliminated by gameplay events. It is used in:

- **Combat Systems**: Determines whether an entity (e.g., an enemy or player) should be destroyed upon taking damage or colliding with another entity.
- **Effect Systems**: Triggers visual or auditory effects, such as explosions or debris, when an entity is destroyed.
- **Projectile Systems**: Handles the removal of projectiles after they collide with targets or exceed their range.
- **Game Logic Systems**: Integrates destruction mechanics with gameplay elements like score increments, power-up drops, or level progression.

For example, in a shoot 'em up game, when an enemy ship is destroyed, the Destruction component can trigger an explosion effect, increase the player's score, and remove the entity from the game world. Similarly, it can ensure that temporary objects like power-ups disappear after a certain amount of time.

### Gun Component
#### Description

The Gun component manages the weapon mechanics of an entity in the game. It provides functionality for controlling various aspects of an entity's gun, such as:

- **bulletDamage**: The amount of damage each bullet deals.
- **fireRate**: The interval between shots, measured in milliseconds.
- **bulletVelocityX**: The horizontal speed of bullets when fired.
- **bulletVelocityY**: The vertical speed of bullets when fired.
- **spriteId**: The identifier for the bullet's visual representation (e.g., sprite or texture).
- **chrono**: A timer to track firing intervals and manage fire rate logic.

This component is essential for entities that can fire projectiles, such as player-controlled ships, enemies, or turrets. It supports serialization and deserialization for network synchronization or saving/loading game states.

#### Use Case

The Gun component is vital for managing the shooting mechanics of an entity. It is used in:

- **Combat Systems**: Defines weapon properties such as damage, fire rate, and bullet speed.
- **Projectile Systems**: Spawns bullets or projectiles with the specified velocity and sprite.
- **Network Synchronization**: Ensures that gun behavior (e.g., shooting and damage) is consistent between the client and server.
- **Game Logic Systems**: Integrates gun behavior with other mechanics like cooldowns or power-ups.

For example, a player’s ship in a shoot 'em up game might have a Gun component defining its primary weapon, while enemy ships may use the same component with different configurations for damage and firing patterns.

### Health Component
#### Description

The Health component manages the health state of an entity in the game. It tracks two key properties:

- **currentHealth**: The current health value of the entity.
- **maxHealth**: The maximum health value the entity can have.

This component is essential for entities that can take damage or recover health, such as players, enemies, or destructible objects. It also supports serialization and deserialization, making it suitable for network synchronization or saving/loading game states.

#### Use Case

The Health component is vital for managing the survival mechanics of an entity. It is used in:

- **Combat Systems**: Tracks damage and health recovery during gameplay.
- **UI Systems**: Displays the health bar or health-related indicators on the screen.
- **Game Logic Systems**: Determines whether an entity is "alive" or "dead" based on its current health.
- **Network Synchronization**: Shares health state between the client and server in multiplayer games.

For example, a player, boss enemy, or destructible environment object would all require a Health component to define their survivability within the game world.

### Position Component
#### Description

The Position component is responsible for managing the 2D spatial positioning of an entity within the game world. It stores the following properties:

- **x**: The horizontal coordinate of the entity.
- **y**: The vertical coordinate of the entity.
- **layer**: The rendering layer to control the entity's depth (e.g., whether it is rendered in front or behind other entities).

This component allows entities to have a precise position in the game environment and can be serialized/deserialized for network transmission or saving/loading game states.

#### Use Case

The Position component is essential for any entity that needs to occupy a specific location in the game world. It is used in:

- **Rendering Systems**: Determines where an entity should be drawn on the screen and its order relative to others.
- **Collision Systems**: Identifies where an entity is located to detect and respond to collisions with other entities.
- **Movement Systems**: Updates the entity’s position based on velocity or other forces.
- **Network Synchronization**: Shares the entity’s position between the client and server in multiplayer games.

For example, a player character, an enemy, or a collectible item would all require a Position component to determine their location within the game world.

### Scale Component
#### Description

The Scale component represents the dimensions of an entity, storing and controlling its width and height. It is useful for handling resizing operations in a variety of game entities like characters, items, or objects that may require dynamic scaling during gameplay. The component supports serialization and deserialization for both single-player and multiplayer environments.

- **Scale Values**: Includes width and height to define the size of the entity.

Use Case

The Scale component is essential in games where entities' sizes need to change dynamically. It supports scalable objects, such as resizing player characters, enemies, or any other game entity. Additionally, it is useful for adapting entity dimensions based on in-game events, such as power-ups or environmental factors.

- **Resizing**: Provides flexibility for entities to change size.
- **Network Integration**: Ensures consistent synchronization of scale values between the client and server in multiplayer games.

For example, in a multiplayer action game, the Scale component allows the player’s avatar or projectiles to dynamically change in size based on power-ups or abilities. Serialization ensures these size changes are consistent across all players, with deserialization making sure the scale value is accurately reflected when entities are transmitted over the network.

### Sound Component
#### Description

The Sound component is responsible for managing sound effects associated with an entity, including properties like sound type, file path, volume, pitch, and looping behavior. It facilitates the dynamic handling of sounds in gameplay, such as character sounds, environmental sounds, or event-triggered audio effects. The component also supports serialization and deserialization, making it ideal for both single-player and multiplayer environments where audio state must be synchronized across clients and the server.

- **Sound Properties**: Includes sound type, file path, volume, pitch, looping behavior, and sound state.

#### Use Case

The Sound component is crucial in games where entities have sounds that need to change based on gameplay events, such as when a player picks up an item or when enemies make noise. It can also be used for more general audio handling, such as background music and ambient effects. Serialization ensures that the sound properties are preserved across different devices and environments in multiplayer games.

- **Dynamic Sound Control**: Useful for games where sounds change dynamically based on gameplay events.
- **Network Synchronization**: Ensures that sound data is consistent between the client and server in multiplayer environments.

For example, in a multiplayer shooter, the Sound component allows the sound effects of gunshots, footsteps, and explosions to be dynamically adjusted. The component’s serialization and deserialization ensure that sound properties like volume or pitch are consistent between all players during gameplay.

### SpriteId Component
#### Description

The SpriteID component is responsible for associating a unique identifier with an entity's sprite. This ID is used to identify and reference sprite resources, which are critical for rendering entities in the game. The component ensures that the sprite ID is serialized and deserialized efficiently for network synchronization, enabling consistent visual representation across clients and servers in multiplayer environments. The fixed-size ID also makes it suitable for use in binary protocols.

- **Sprite Properties**: Includes a unique string identifier for the sprite with a fixed size of 20 bytes for network compatibility.

#### Use Case

The SpriteID component is essential for games where entities require unique visual representations tied to their functionality or gameplay context. It ensures consistent and efficient management of sprite resources across both single-player and multiplayer scenarios. Serialization and deserialization enable synchronization of sprite data across different systems.

- **Dynamic Resource Mapping**: Allows dynamic loading and switching of sprites during gameplay based on events or state changes.
- **Network Synchronization**: Ensures that sprite identifiers are consistent between the client and server in multiplayer environments.

For example, in a side-scrolling shooter game, the SpriteID component allows each enemy type to be linked to a specific sprite (e.g., "enemy_01") while maintaining network efficiency and reliability. The serialization ensures that all connected clients display the correct sprites during gameplay.

### Type Component
#### Description

The Type component assigns a specific classification or role to an entity in the game. This classification is defined by the TypeID enum, which includes roles like ENEMY, ALLY, and BACKGROUND. The component is crucial for defining an entity's behavior, interaction logic, and visual representation within the game.

- **Type Properties**: TypeID is an enumeration representing the entity type, stored as a single byte (uint8_t).

#### Use Case

The Type component is useful for categorizing and identifying entities in games, enabling systems to process them accordingly. For example, collision systems can treat enemies and projectiles differently based on their TypeID.

- **Behavior Differentiation**: Allows game systems to apply distinct logic based on entity type (e.g., enemies taking damage, allies healing).
- **Network Efficiency**: The TypeID is serialized as a single byte for compact and efficient communication over the network.

In a top-down shooter game, the Type component can classify entities as ENEMY_PROJECTILE or ALLY_PROJECTILE, ensuring that only enemy projectiles can damage player-controlled entities.

### Velocity Component
#### Description

The Velocity component represents the movement of an entity by storing its horizontal (x) and vertical (y) speed, as well as a diminishing factor for controlling velocity decay. This component is essential for enabling dynamic movement, handling acceleration, and controlling deceleration or friction in the game world.

- **x**: The horizontal velocity component, stored as a 32-bit integer.
- **y**: The vertical velocity component, stored as a 32-bit integer.
- **floatX**: The x-component stored as a floating-point number for precise calculations.
- **floatY**: The y-component stored as a floating-point number for precise calculations.
- **diminishingFactor**: An 8-bit unsigned integer representing the rate at which velocity diminishes, useful for simulating friction or drag.

#### Use Case

The Velocity component enables systems to move entities in the game by updating their position based on velocity values. It also provides mechanisms for deceleration through the diminishing factor.

- **Movement Simulation**: Facilitates smooth and dynamic entity movement by applying velocity to position components in a system.
- **Realism with Friction**: Simulates natural deceleration or drag by reducing velocity over time using the diminishing factor.
- **Precision**: Provides both integer and floating-point representations for optimized calculations.

For example, in a space shooter game, the Velocity component can be used to move the player’s spaceship and projectiles, with the diminishing factor allowing for smooth deceleration when the spaceship slows down or stops.

### Visible Component
#### Description

The Visible component determines whether an entity is displayed in the game world. It stores a boolean value (isVisible) that represents the entity's visibility state. This component can be serialized and deserialized for network communication, making it crucial for dynamic gameplay visuals and multiplayer synchronization.

- **isVisible**: A boolean value indicating whether the entity is visible (true) or invisible (false).

#### Use Case

The Visible component is essential for managing the rendering state of entities in the game. It allows systems to efficiently handle visible and invisible entities, enabling features such as hiding, showing, or toggling the visibility of entities dynamically.

- **Dynamic Visibility**: Enables real-time control over whether an entity is rendered in the game world.
- **Optimized Rendering**: Helps improve rendering performance by excluding invisible entities from rendering pipelines.
- **Multiplayer Synchronization**: Facilitates synchronization of visibility states across clients in a multiplayer game.

For example, in a multiplayer game, the Visible component can hide certain entities, such as power-ups, from players who are not eligible to see them. It can also be used to create visual effects, such as making an entity disappear when it enters stealth mode.

## Version

R-Typeuh component plug-ins documentation version 0.1.0, 23/01/2024, written by Tibaud Le Maout
