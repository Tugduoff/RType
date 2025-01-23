# GameEngine Documentation

## Overview

The GameEngine is a highly modular framework designed for game development. It utilizes a system of plugins, which include components and systems, to provide flexibility and scalability. The engine manages entities identified by unique IDs and allows developers to build games by assembling the required components and systems.

## Core Concepts

### 1. **Entities**
Entities are the fundamental building blocks of any game built with the GameEngine. Each entity is assigned a unique ID starting from 0, and new entities can be created dynamically as needed.

### 2. **Components**
Components define the data associated with an entity. Examples of components include:
- **Position Component**: Stores the position of an entity in the game world.
- **Acceleration Component**: Stores acceleration data to calculate movement.

### 3. **Systems**
Systems define the behavior and logic that operate on entities with specific components. Examples of systems include:
- **Move System**: Enables entities with Position and Acceleration components to move.
- **SFML System**: Provides rendering capabilities using the SFML library.
- **Entity Creation System**: Handles the creation of new entities.

From this point onwards, we will create a very simple game in which we can move our player. If you didn't already did, I invite you to read the `component_documentation.md` and `system_documentation.md`.

## Creating a Simple Game

This section explains how to create a basic game using the GameEngine.

### Step 1: Define Components

#### Position Component
##### Position.hpp:
```cpp
#ifndef POSITION_HPP
#define POSITION_HPP

#include "GameEngine/GameEngine.hpp"
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
##### Position.cpp:
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

#### Acceleration Component
##### Position.hpp:
```cpp
/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Acceleration.hpp file
*/

#ifndef ACCELERATION_HPP
    #define ACCELERATION_HPP

    #include "GameEngine/GameEngine.hpp"
    #include "components/AComponent.hpp"

namespace Components {
    class Acceleration : public AComponent<Acceleration> {
    public:
        Acceleration(int32_t forward = 2, int32_t backward = 1, int32_t right = 1, int32_t left = 1) :
            AComponent("Acceleration"), forward(forward), backward(backward), right(right), left(left) {
            };
        ~Acceleration() = default;

        /**
         * @brief Serializes the acceleration components into a byte vector.
         * 
         * Converts the forward, backward, right, and left acceleration components
         * into network byte order for transmission or storage.
         * 
         * @return A vector of bytes representing the serialized acceleration component.
         */
        std::vector<uint8_t> serialize() override {
            __network.forward = htonl(forward);
            __network.backward = htonl(backward);
            __network.right = htonl(right);
            __network.left = htonl(left);
            return std::vector<uint8_t>(__data, __data + sizeof(__data));
        };

        /**
         * @brief Deserializes the acceleration component from the provided byte vector.
         * 
         * Reads the forward, backward, right, and left acceleration components in network byte order.
         * 
         * @param data A vector of bytes representing the serialized acceleration component.
         * @throws std::runtime_error If the data size is invalid.
         */
        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != sizeof(__data))
                throw std::runtime_error("Invalid data size for position component");

            forward = ntohl(*reinterpret_cast<int32_t *>(data.data()));
            backward = ntohl(*reinterpret_cast<int32_t *>(data.data() + 4));
            right = ntohl(*reinterpret_cast<int32_t *>(data.data() + 8));
            left = ntohl(*reinterpret_cast<int32_t *>(data.data() + 12));
        };

        /**
         * @brief Gets the size of the serialized data.
         * 
         * @return The size of the data, in bytes.
         */
        size_t getSize() const override {
            return sizeof(__data);
        };

        /**
         * @brief Adds the Acceleration component to an entity.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param args The arguments to pass to the component constructor.
         * 
         * @note This function will add the component to the entity.
         * @note The arguments should be the forward, backward, right, and left acceleration values, all int32_t.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() != 4)
                throw std::runtime_error("Invalid number of arguments for Acceleration component");

            int32_t forward = std::any_cast<int32_t>(args[0]);
            int32_t backward = std::any_cast<int32_t>(args[1]);
            int32_t right = std::any_cast<int32_t>(args[2]);
            int32_t left = std::any_cast<int32_t>(args[3]);

            engine.getRegistry().componentManager().addComponent<Components::Acceleration>(to, engine.newComponent<Components::Acceleration>(forward, backward, right, left));
        };

        /**
         * @brief Adds the Acceleration component to an entity.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param config The configuration settings to use for the component.
         * 
         * @note This function will add the component to the entity.
         * @note The configuration settings should contain the forward, backward, right, and left acceleration values.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            int forwardVal = 0, backwardVal = 0, rightVal = 0, leftVal = 0;

            if (!config.lookupValue("forward", forwardVal)) {
                std::cerr << "Warning: 'forward' not found in config. Using default value: 0\n";
                forwardVal = 2;
            }

            if (!config.lookupValue("backward", backwardVal)) {
                std::cerr << "Warning: 'backward' not found in config. Using default value: 0\n";
                backwardVal = 1;
            }

            if (!config.lookupValue("right", rightVal)) {
                std::cerr << "Warning: 'right' not found in config. Using default value: 0\n";
                rightVal = 1;
            }

            if (!config.lookupValue("left", leftVal)) {
                std::cerr << "Warning: 'left' not found in config. Using default value: 0\n";
                leftVal = 1;
            }

            std::cout << "forward: " << forwardVal << " backward: " << backwardVal << " right: " << rightVal << " left: " << leftVal << std::endl;

            attachAndUpdateComponent<Components::Acceleration>(
                engine, to,
                static_cast<int32_t>(forwardVal),
                static_cast<int32_t>(backwardVal),
                static_cast<int32_t>(rightVal),
                static_cast<int32_t>(leftVal));
        };

        int32_t forward;
        int32_t backward;
        int32_t right;
        int32_t left;

    private:
        union {
            struct {
                int32_t forward;
                int32_t backward;
                int32_t right;
                int32_t left;
            } __network;
            uint8_t __data[16];
        };
    };
};

#endif // ACCELERATION_HPP
```

### Step 2: Implement Systems

#### Move System
The Move System updates the position of entities based on their acceleration.
```cpp
Put the move system here
```

#### SFML System
The SFML System renders entities with a Position component.
```cpp
Puth the SFML system here (simple system, maybe handle inputs)
```

#### Entity Creation System
```cpp
Put simple system here that creates one entity
```

### Step 3: Integrate Components and Systems

#### Main Function
```cpp
#include "GameEngine/GameEngine.hpp"

int main() {
    Engine::GameEngine engine;

    try {
        while (true) {
            if (chrono.getElapsedTime() < 17)
                continue;
            engine.runSystems();
            chrono.restart();
       }

    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
```

## Conclusion

The GameEngine's modular design simplifies game development by allowing developers to focus on defining components and systems. By following the steps outlined above, you can create a simple game and expand upon it with additional components and systems as needed.

