# GameEngine Documentation

## Overview

Our GameEngine is designed to work like an ECS. An ECS is an entity component system, meaning that for creating a game, you only need : components and systemes in order to create your entities. 

In order to do that, we utilize plugins wherever we can. It provides flexibility for the game developpement and easy structuration.

## Core Concepts

### 1. **Entities**
Entities are the fundamental building blocks of any game built with the GameEngine. Each entity is assigned a unique ID starting from 0, and new entities can be created dynamically as needed. When thinking of an entity, think of it as an index that will be used to retrieve its components.

### 2. **Components**
Components define the data associated with an entity. Examples of components include:
- **Position Component**: Stores the position of an entity in the game world.
- **Acceleration Component**: Stores acceleration data to calculate movement.
Components are simple building blocks used to store data. They do not incorporate any logic and they shouldn't.

### 3. **Systems**
Systems define the behavior and logic that operate on entities with specific components. Examples of systems include:
- **Move System**: Enables entities with Position and Acceleration components to move.
- **SFML System**: Provides rendering capabilities using the SFML library.
- **Entity Creation System**: Handles the creation of new entities.
Systemes are more complex and some of them are specificly made for a game.

In order to explain as much as possible our system, we will provide below, a step by step instruction on how to use and modify our example game in order to gain as much info as possible for when you will be ready to start creating your first game!

If you didn't already did, I invite you to read the `component_documentation.md` and `system_documentation.md`.

## Creating a Simple Game

Our example game is inspired from leaf blower revolution.
Therefore, the first step for this one and for each game that is created should be to define the rules:

Elements:
- A player spawns in the middle of the screen with a texture and rotation set to face the right of the screen.
- Leaves spawn at random positions during the game

Events:
- Whenever the player touches a leaf, it disapears and the player gains a point.
- Whenever the user presses the escape key, it closes the game

UI:
- A score bar is displayed on the top left corner.

That's basically it, let's start coding !


We will start by browsing our library of components, for this example, we have used only 5 of them : 

- Position (in a 2d space)
- Type (defines the entity type)
- Rotation (in a 2d or 3d space)
- Collider (in a 2d space)
- Score (stores a value)

You can check thoses files in the `plugins/components/` section of our project, but we will explain them a bit below!

If you'd like to skip, go to [`Step 2`](#step-2-implement-systems)

### Step 1: Define Components

For each component, refers the the `component_documentation` to know how they work.
Theses use the IComponent interface below : 

#### IComponent Interface
```cpp
namespace Components {
    class IComponent {
        public:
            virtual ~IComponent() = default;

            virtual std::vector<uint8_t> serialize() = 0;
            virtual void deserialize(std::vector<uint8_t> &data) = 0;
            virtual size_t getSize() const = 0;
            virtual void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) = 0;
            virtual void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) = 0;
            virtual SparseArray<IComponent> &any_cast(std::any &sparseArray) = 0;
            virtual const SparseArray<IComponent> &any_cast(const std::any &sparseArray) = 0;
            virtual const std::string &getId() = 0;

        private:
    };
};
```

#### Position Component
##### Position.hpp:
```cpp
namespace Components {
    class Position : public AComponent<Position> {
        private:
            uint32_t x, y, layer; // Has 3 variables : x, y and layer.
    };
}
```

#### Type Component
##### Type.hpp:
```cpp
enum class TypeID : uint8_t {
    ENEMY = 0,
    ALLY,
    ALLY_PROJECTILE,
    ENEMY_PROJECTILE,
    BACKGROUND,
    UI,
    UNKNOWN = 255 // Optional: Handle an invalid state
};

namespace Components {
    class Type : public AComponent<Type> {
        private:
            TypeID id;
    };
}
```

Stores this enumeration for the types of the entities on the game, it's supposed to be generic enough to be used in most games.

#### Rotation Component
##### Rotation.hpp:

```cpp
namespace Components {
    class Rotation : public AComponent<Rotation> {
        private:
            int32_t x;
            int32_t y;
            int32_t z;
            float floatX;
            float floatY;
            float floatZ;
    };
}
```

This component stores raw x, y and z values storing the yaw, roll and pitch values of the rotation as well as their float representatives for smoother rotations.

#### Collider Component
##### Collider.hpp:

```cpp
namespace Components {
    class Collider : public AComponent<Collider> {
        private:
            uint32_t width;
            uint32_t height;
    };
}
```

This component stores the width and height of the collider of the entity.

#### Score Component
##### Score.hpp:

```cpp
namespace Components {
    class Collider : public AComponent<Collider> {
        private:
            uint32_t value;
    };
}
```

This component stores the value of the score in a simple unsigned int variable.

### Step 2: Implement Systems

Now that we have seen each component that will be used, le'ts focus on the systems.

Below are the name of each system that will be used.

- Display
- Spawn
- Collision
- Move

therefore, here is the configuration needed to use each of them:

```json
{
    dir = "",
    name = "LeafBlowerDisplaySystem",
    args = {
        width = 1920,
        height = 1080,
        title = "LeafBlower",
        fullscreen = true,
    }
},
{
    dir = "",
    name = "LeafBlowerSpawnSystem",
    args = {
        speed = 1000,
        maxLeaves = 100000,
        width = 1920,
        height = 1080,
    }
},
{
    dir = "",
    name = "LeafBlowerCollisionSystem",
    args = {
        playerForce = 1,
    }
},
{
    dir = "",
    name = "LeafBlowerMoveSystem",
    args = {
        speed = 10,
    }
}
```

This configuration file will call each system with their properties and arguments.

#### Move System
The Move System updates the position of the player based on the given speed.

```cpp
void Systems::Move::run(Engine::GameEngine &engine)
{
    auto &componentManager = engine.getRegistry().componentManager();
    auto &posComponentArr = componentManager.getComponents<Components::Position>();
    auto &typeComponentArr = componentManager.getComponents<Components::Type>();
    auto &rotComponentArr = componentManager.getComponents<Components::Rotation>();

    int playerIndex = -1;

    for (auto &&[i, type] : IndexedZipper(typeComponentArr)) {
        if (type.id == Components::TypeID::ALLY) {
            playerIndex = i;
        }
    }

    if (playerIndex == -1) {
        std::cerr << "No player found" << std::endl;
        return;
    }

    // check where the cursor is and move the player to the cursor
    Vector2 cursor = GetMousePosition();

    try {
        auto &pos = posComponentArr[playerIndex];
        auto &rot = rotComponentArr[playerIndex];

        // Smooth movement using linear interpolation (lerp)
        const float lerpFactor = 0.1f;
        pos->x += (cursor.x - pos->x) * lerpFactor * __speed / 10;
        pos->y += (cursor.y - pos->y) * lerpFactor * __speed / 10;

        // Rotate the player towards the cursor
        float angle = atan2(cursor.y - pos->y, cursor.x - pos->x) * RAD2DEG;
        rot->z = angle;

        DrawCircle(pos->x, pos->y, 80, DARKGREEN);
        // DrawCircle(cursor.x, cursor.y, 40, DARKGREEN);

    } catch (std::out_of_range &e) {
        std::cerr << "Player position not found" << std::endl;
    }
}
```

It takes as argument the speed of the movement: `uint32_t __speed;`

#### Display System
The Display System renders all our entities.

```cpp
void Systems::Display::run(Engine::GameEngine &engine)
{
    auto &componentManager = engine.getRegistry().componentManager();
    auto &posComponentArr = componentManager.getComponents<Components::Position>();
    auto &typeComponentArr = componentManager.getComponents<Components::Type>();
    auto &rotComponentArr = componentManager.getComponents<Components::Rotation>();
    auto &scoreComponentArr = componentManager.getComponents<Components::Score>();

    if (WindowShouldClose()) {
        CloseWindow();
        exit(0);
    }

    ClearBackground(LIME);
    BeginDrawing();

    // Ally is the player, enemies are the leaves
    for (auto &&[pos, type, rot] : Zipper(posComponentArr, typeComponentArr, rotComponentArr)) {
        if (type.id == Components::TypeID::ENEMY) {
            Vector2 origin = {21, 21};

            DrawTexturePro(__leafTexture,
                {0, 0, 42, 42},
                {(float)pos.x, (float)pos.y, 42, 42},
                origin, rot.z, RAYWHITE);
        }
    }
    for (auto &&[pos, type, rot, score] : Zipper(posComponentArr, typeComponentArr, rotComponentArr, scoreComponentArr)) {
        if (type.id == Components::TypeID::ALLY) {
            Vector2 origin = {80, 80};

            DrawTexturePro(__playerTexture,
                {0, 0, 160, 160},
                {(float)pos.x, (float)pos.y, 160, 160},
                origin, rot.z, RAYWHITE);

            std::string scoreStr = "Score: " + std::to_string(score.value);
            DrawText(scoreStr.c_str(), 10, 10, 20, BLACK);
        }
    }

    EndDrawing();
}
```

#### Entity Spawn System
```cpp

void Systems::Spawn::run(Engine::GameEngine &engine)
{
    auto &componentManager = engine.getRegistry().componentManager();
    auto &typeComponentArr = componentManager.getComponents<Components::Type>();

    uint32_t leaves = 0;

    // Count the number of leaves
    for (auto &&[type] : Zipper(typeComponentArr)) {
        if (type.id == Components::TypeID::ENEMY) {
            leaves++;
        }
    }

    if (leaves < __maxLeaves) {
        for (uint32_t i = 0; i < __speed && leaves < __maxLeaves; i++) { // Spawn leaves based on speed
            spawnLeafAtRandomPosition(engine);
            leaves++;
        }
    }
}

void Systems::Spawn::spawnLeafAtRandomPosition(Engine::GameEngine &engine)
{
    ECS::Entity newLeaf = engine.getRegistry().createEntity();

    uint32_t randomX = rand() % __width;
    uint32_t randomY = rand() % __height;

    Components::Position pos(randomX, randomY, 0); // Leaves are on layer 0
    Components::Type type(Components::TypeID::ENEMY);
    Components::Collider collider(42, 42); // Leaves are 42x42px
    Components::Rotation rotation(0, 0, rand() % 360);

    engine.getRegistry().componentManager().addComponent(newLeaf, std::make_unique<Components::Position>(pos));
    engine.getRegistry().componentManager().addComponent(newLeaf, std::make_unique<Components::Type>(type));
    engine.getRegistry().componentManager().addComponent(newLeaf, std::make_unique<Components::Collider>(collider));
    engine.getRegistry().componentManager().addComponent(newLeaf, std::make_unique<Components::Rotation>(rotation));
}

// And the init function:
void Systems::Spawn::init(Engine::GameEngine &engine)
{
    engine.registerComponent<Components::Position>("./plugins/bin/components/", "Position");
    engine.registerComponent<Components::Type>("./plugins/bin/components/", "Type");
    engine.registerComponent<Components::Collider>("./plugins/bin/components/", "Collider");
    engine.registerComponent<Components::Rotation>("./plugins/bin/components/", "Rotation");
    engine.registerComponent<Components::Score>("./plugins/bin/components/", "Score");

    srand(time(NULL));

    // Spawn the player in the middle of the screen, supposedly at 960, 540
    // but can change depending on width and height
    ECS::Entity player = engine.getRegistry().createEntity();

    Components::Position pos(__width / 2, __height / 2, 1); // Player is on layer 1
    Components::Type type(Components::TypeID::ALLY);
    Components::Collider collider(160, 160); // Player is 160x160px
    Components::Rotation rotation(0, 0, 0);
    Components::Score score(0);

    engine.getRegistry().componentManager().addComponent(player, std::make_unique<Components::Position>(pos));
    engine.getRegistry().componentManager().addComponent(player, std::make_unique<Components::Type>(type));
    engine.getRegistry().componentManager().addComponent(player, std::make_unique<Components::Collider>(collider));
    engine.getRegistry().componentManager().addComponent(player, std::make_unique<Components::Rotation>(rotation));
    engine.getRegistry().componentManager().addComponent(player, std::make_unique<Components::Score>(score));

    uint32_t leaves = 0;

    // Spawn starting 10 leaves
    for (int i = 0; i < 10 && leaves < __maxLeaves; i++) {
        spawnLeafAtRandomPosition(engine);
        leaves++;
    }
}
```

#### Collision System

```cpp
void Systems::Collision::run(Engine::GameEngine &engine)
{
    auto &componentManager = engine.getRegistry().componentManager();
    auto &posComponentArr = componentManager.getComponents<Components::Position>();
    auto &typeComponentArr = componentManager.getComponents<Components::Type>();
    auto &colliderComponentArr = componentManager.getComponents<Components::Collider>();
    auto &scoreComponentArr = componentManager.getComponents<Components::Score>();

    std::unique_ptr<ECS::Entity> player;
    std::vector<ECS::Entity> leaves;

    for (auto &&[i, type] : IndexedZipper(typeComponentArr)) {
        switch (type.id) {
            case Components::TypeID::ALLY:
                player = std::make_unique<ECS::Entity>(i);
                break;
            case Components::TypeID::ENEMY:
                leaves.push_back((ECS::Entity)i);
                break;
            default:
                break;
        }
    }

    if (!player) {
        std::cerr << "No player found" << std::endl;
        return;
    }

    // Check the collision between the player collider and the leaf collider for each leaf in leaves
    auto &playerCollider = colliderComponentArr[*player];
    auto &playerPos = posComponentArr[*player];
    auto &playerScore = scoreComponentArr[*player];

    int posX = playerPos->x - 80;
    int posY = playerPos->y - 80;

    // DrawRectangle(posX, posY, playerCollider->width, playerCollider->height, RED);

    for (auto &leaf : leaves) {
        auto &leafPos = posComponentArr[leaf];
        auto &leafCollider = colliderComponentArr[leaf];

        int leafPosX = leafPos->x;
        int leafPosY = leafPos->y;

        if (leafPosX < posX + (int)playerCollider->width &&
            leafPosX + (int)leafCollider->width > posX &&
            leafPosY < posY + (int)playerCollider->height &&
            leafPosY + (int)leafCollider->height > posY) {
            playerScore->value += 1;
            engine.getRegistry().killEntity(leaf);
        }
    }
}
```
