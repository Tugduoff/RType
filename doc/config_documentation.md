# Project Configuration File Documentation

This document provides an overview of the configuration options available in the project’s configuration file, detailing each section, parameter, and component.

## Table of Contents

1. [Level](#level)
2. [Templates](#templates)
3. [Entities](#entities)

## Level

The `level` section defines the game’s general settings and environment.

- **name**: *(string)* Name of the level.
- **description**: *(string)* Brief description of the level.
- **difficulty**: *(string)* Level difficulty, e.g., `EASY`, `MEDIUM`, `HARD`.
- **backgroundMusic**: *(string)* Path to the background music file.
- **cell_size**: *(integer)* Size of each map cell in pixels.
- **map_size**: *(object)* Dimensions of the map.
  - **x**: *(integer)* Width of the map in cells.
  - **y**: *(integer)* Height of the map in cells.
- **view_port**: *(object)* Screen dimensions for the viewport.
  - **x**: *(integer)* Width of the viewport in pixels.
  - **y**: *(integer)* Height of the viewport in pixels.
- **goal**: *(string)* Objective or goal of the level.

## Templates

The `templates` section allows defining reusable entity configurations, such as enemies, with a list of components and their settings that will be used as their default values.

### Template Structure

Each template has the following structure:

- **name**: *(string)* Template identifier, e.g., `"Enemy"`.
- **components**: *(array)* List of components associated with the template.

#### Components

Each component within a template includes:

- **name**: *(string)* Component type, e.g., `Position`, `Velocity`, `Collider`.
- **args**: *(object)* Arguments for component configuration.

#### Example Components

1. **Position**
   - **x**: *(integer)* X-coordinate.
   - **y**: *(integer)* Y-coordinate.
   - **layer**: *(integer)* Rendering layer.

2. **SpriteID**
   - **id**: *(string)* ID of the sprite asset.

3. **Velocity**
   - **x**: *(integer)* Horizontal speed.
   - **y**: *(integer)* Vertical speed.
   - **factor**: *(integer)* Velocity scaling factor.

4. **Sound**
   - **sounds**: *(array)* List of sound configurations.
     - **id**: *(string)* Sound identifier, e.g., `ATTACK`.
     - **path**: *(string)* Path to the sound file.
     - **volume**: *(integer)* Volume level.
     - **pitch**: *(float)* Sound pitch.
     - **loop**: *(boolean)* Whether the sound should loop.

## Entities

The `entities` section defines game objects placed within the level. Each entity can either be configured directly or reference a template from the `templates` section.

### Entity Structure

Each entity has the following structure:

- **name**: *(string)* Unique identifier for the entity.
- **type**: *(string, optional)* Name of a template to inherit components.
- **spawnTime**: *(integer)* Time in milliseconds for the entity to appear.
- **components**: *(array)* List of specific components and their settings.

#### Example Entity: Player

- **Position**
  - **x**: *(integer)* X-coordinate.
  - **y**: *(integer)* Y-coordinate.
  - **layer**: *(integer)* Rendering layer.

- **Controllable**
  - **FORWARD**: *(string)* Key binding for moving forward.
  - **BACKWARD**: *(string)* Key binding for moving backward.
  - **LEFT**: *(string)* Key binding for moving left.
  - **RIGHT**: *(string)* Key binding for moving right.

- **Gun**
  - **bulletDamage**: *(integer)* Damage dealt per bullet.
  - **fireRate**: *(integer)* Rate of fire in milliseconds.
  - **bulletVelocityX**: *(integer)* Horizontal bullet speed.

## Syntax Example

Below is an example of the configuration file format, demonstrating how to structure the `level`, `templates`, and `entities` sections.

example.cfg:

```cfg
level = {
    name = "Forest Adventure",
    description = "A peaceful forest area with simple enemies to introduce basic game mechanics.",
    difficulty = "EASY",
    backgroundMusic = "./assets/background.mp3",
    cell_size = 30,
    map_size = { x = 1000, y = 10 },
    view_port = { x = 1920, y = 1080 },
    goal = "Reach the exit while avoiding enemy patrols."
}

templates = (
    {
        name = "Enemy",
        components = (
            {
                name = "Position",
                args = {
                    x = 1000,
                    y = 500,
                    layer = 2
                }
            },
            {
                name = "Velocity",
                args = {
                    x = -1,
                    y = 0,
                    factor = 100
                }
            },
            // Additional components...
        )
    }
    // Additional template...
)

entities = (
    {
        name = "Player",
        spawnTime = 0,
        components = (
            {
                name = "Position",
                args = {
                    x = 250,
                    y = 500,
                    layer = 2
                }
            },
            {
                name = "Controllable",
                args = {
                    FORWARD = "D",
                    BACKWARD = "Q",
                    LEFT = "Z",
                    RIGHT = "S",
                    ACTION1 = "SPACE"
                }
            },
            // Additional components...
        )
    }
    // Additional entities
)
```

## Notes

- **File Paths**: Paths are relative to the project root unless otherwise specified.
- **Coordinate System**: (0,0) refers to the top-left corner of the window.

This document serves as a quick reference to configure levels, entities, and reusable templates effectively. Modify values as per your game requirements and ensure paths and identifiers match the project's assets and logic.

## Version

R-Typeuh configuration file documentation version 0.1.1, 23/01/2024, written by Tibaud Le Maout