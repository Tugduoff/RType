# Component Plugins Developer Documentation

## Purpose

This documentation is intended for developers who are creating **new component plugins** for the **RType** game engine. Components define the data that will be associated for each entity within the game. This guide explains how to extend the `IComponent` interface, serialize/deserialize component data, and add components to entities, making your custom component fully compatible with the engine.

By following this guide, you will be able to create a new plugin that integrates seamlessly into the RType ECS (Entity Component System) architecture.

---

## IComponent Interface

To add a new component as a plugin, you need to **inherit from the `AComponent`, itself inheriting from `IComponent` interface** and implement the required methods. Below is an overview of the `IComponent` interface and step-by-step instructions on how to implement it.

### Key Methods to Implement

Your custom component must define the following methods to work as a plugin within the RType engine:

### 1. `serialize() -> std::vector<uint8_t>`
- **Purpose**: Serialize the component's data into a format that can be transmitted through our network protocol. It must result as a static size packet, with each elements following each other in memory.
- **Return Type**: `std::vector<uint8_t>`
- **Details**:
  - Implement this to **convert your component's internal state** (data members) into a byte array.
  - This allows the engine to **transfer** the component data when needed.
  - The size of the returned vector should match the size of the component's data members concatenated in a single data packet.

### 2. `deserialize(std::vector<uint8_t> &data)`
- **Purpose**: Reconstruct the component's state from serialized data.
- **Parameters**: 
  - `data`: A byte stream (vector of bytes) containing the serialized state of the component.
- **Details**:
  - Implement this to take in the byte data and **rebuild the component's state**. This is the reverse of `serialize()`.
  - This method is used when receiving component update from the network. It should replace all data members with the updated data.
  - Make sure to validate the size of the data given as argument. It should be the same as the components data size. If not, **throw a `std::runtime_error`**.

### 3. `getSize() const -> size_t`
- **Purpose**: Return the size of the component in bytes.
- **Return Type**: `size_t`
- **Details**:
  - You need to accurately define this method to return the **memory size of your component**.
  - This ensures that the network functions will read and write the same amount of bytes that's needed for your component to work.

### 4. `addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args)`
- **Purpose**: Add the component to a game entity using dynamic arguments. This function must implement 3 parts: `reading args`, `creating instance` and `adding instance to entity`.
- **Parameters**:
  - `to`: The entity to which the component will be added.
  - `engine`: Reference to the game engine.
  - `args`: A vector of arguments passed to the component's constructor.
- **Details**:
  - This method is responsible for **dynamically adding** your component to an entity within the game engine.
  - You should implement this in order to be able to dynamically create and add a component to an entity. This will be used in systems run function as its not possible to create templated components at runtime.

### 5. `addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config)`
- **Purpose**: Add the component to an entity using configuration settings. This function must implement 3 parts: `reading args`, `creating instance` and `adding instance to entity`.
- **Parameters**:
  - `to`: The entity to which the component will be added.
  - `engine`: The game engine context.
  - `config`: A `libconfig::Setting` object containing the configuration data.
- **Details**:
  - This variant of `addTo` is used when initializing a component from an **external configuration file** and we only have the libconfig setting element.

---

## Additional methods in the source file of the component

### 1. `entryPoint() -> std::unique_ptr<Components::IComponent>`
- **Purpose**: This is a factory function required by the engine to dynamically load your component as a plugin. This function will return an instance of the component.
- **Args**: This function can take any args it wants, such as strings, ints and custom structs. Thoses arguments can be use to create the instance by calling a custom constructor.
- **Return Type**: `std::unique_ptr<Components::IComponent>`
- **Details**:
  - Implement this function to **return a new instance** of your custom component.
  - The engine uses this entry point to instantiate your component at runtime.

### 2. `entryConfig() -> std::unique_ptr<Components::IComponent>`
- **Purpose**: Similarily to `entryPoint()`, this factory function also returns an instance of the component but creates the instance using a libconfig setting.
- **Args**: This function takes a libconfig setting element that can be passed and parsed in the constructor of the instance. It allows for defining component's data in config files.
- **Return Type**: `std::unique_ptr<Components::IComponent>`
- **Details**:
  - Implement this function to **return a new instance** of your custom component.
  - The engine uses this entry point to instantiate your component at runtime.

### 3. `char const *componentName`
- **Purpose**: This variable defines the name of the component. This id will be used as a key in a map of component's instances created on registration. It allows for the functions addTo() of the component interface to be called on every component as long as we have the component name. It must be named : componentName

## Steps to Create a New Component Plugin

1. **Create a New Class**:
   - Create a new folder in plugins/components/
   - Inside this folder, create a new file with the name of your component.
   - Define your component's class that **inherits from `IComponent`**.
   - Example:
     ```cpp
     class MyCustomComponent : public Components::IComponent {
         // Implement all methods from the IComponent interface.
     };
     ```

2. **Implement the Required Methods**:
   - Ensure that your component correctly implements `serialize()`, `deserialize()`, `getSize()`, and the `addTo()` methods.
   - Without theses functions, an error will occur indicating the missing function(s)

3. **Add Your Plugin to the Engine and Make it Work**:
   - Once your component is created, in order to compile it, you need to create a new `CMakeLists.txt` file in your custom folder containing the following : 
   ```
    # Add an executable for the server project
    add_library([Name of executable] SHARED
        [Name of file].cpp
    )
    ```
   - Feel free to name your executable the way you want, but name of file must be the name of your custom component's file.
   - Implement your component in one of our systems or your custom system and have fun!

---

## Example: Simple Custom Component Plugin

Here's a simple example of how you might implement a basic custom component as a plugin:

### Header file: `HealthComponent.hpp`
```cpp
/*
** EPITECH PROJECT, 2024
** RType
** File description:
** HealthComponent.hpp file
*/

#ifndef HEALTH_COMPONENT_HPP
    #define HEALTH_COMPONENT_HPP

    #include "components/AComponent.hpp"
    #ifdef _WIN32
        #include <winsock2.h>
    #else
        #include <arpa/inet.h>
    #endif
    #include <vector>
    #include <stdexcept>

namespace Components {
    /**
     * @brief Health component class for ECS.
     * 
     * This class represents the health of an entity, storing the health value and
     * providing methods to serialize and deserialize the health data.
     */
    class HealthComponent : public AComponent {
        public:
            /**
             * @brief Default constructor for the Health component.
             * 
             * Initializes the health value to 100 by default.
             */
            HealthComponent() : health(100) {};

            /**
             * @brief Parameterized constructor for the Health component.
             * 
             * Initializes the health with the provided value.
             * 
             * @param health The initial health value.
             */
            HealthComponent(uint32_t health) : health(health) {};

            /**
             * @brief Default destructor for the Health component.
             */
            ~HealthComponent() = default;

            /**
             * @brief Serializes the health component into a byte vector.
             * 
             * Converts the health value into network byte order for transmission or storage.
             * 
             * @return A vector of bytes representing the serialized health value.
             */
            std::vector<uint8_t> serialize() override {
                __network.health = htonl(health);
                return std::vector<uint8_t>(__data, __data + sizeof(__data));
            };

            /**
             * @brief Deserializes the health value from the provided byte vector.
             * 
             * Reads the health value in network byte order.
             * 
             * @param data A vector of bytes representing the serialized health value.
             * @throws std::runtime_error If the data size is invalid.
             */
            void deserialize(std::vector<uint8_t> &data) override {
                if (data.size() != sizeof(__data))
                    throw std::runtime_error("Invalid data size for health component");
                health = ntohl(*reinterpret_cast<uint32_t *>(data.data()));
            };

            /**
             * @brief Gets the size of the serialized data.
             * 
             * @return The size of the data, in bytes.
             */
            size_t getSize() const override { return sizeof(__data); };

            uint32_t health;

        private:
            union {
                struct {
                    uint32_t health;
                } __network;
                uint8_t __data[4]; // Health is 4 bytes (32 bits)
            };
    };
};

#endif // HEALTH_COMPONENT_HPP
```

### Source file: `HealthComponent.cpp`

```cpp
/*
** EPITECH PROJECT, 2024
** RType
** File description:
** HealthComponent.cpp file
*/

#include "HealthComponent.hpp"
#include "library_entrypoint.hpp"

/**
 * @brief Entry point function for creating a new HealthComponent with a specified health value.
 * 
 * This function is used to create an instance of HealthComponent with a provided initial health value.
 * 
 * @param health The initial health value for the component.
 * @return Components::IComponent* Pointer to the newly created HealthComponent instance.
 */
LIBRARY_ENTRYPOINT
Components::IComponent *entryPoint(uint32_t health)
{
    return new Components::HealthComponent(health);
}

/**
 * @brief Entry point function for creating a new HealthComponent using configuration data.
 * 
 * This function is used to create an instance of HealthComponent based on settings provided
 * in a libconfig::Setting object. The configuration is used to initialize the component's state.
 * 
 * @param config A libconfig::Setting object containing the configuration data.
 * @return Components::IComponent* Pointer to the newly created HealthComponent instance.
 */
LIBRARY_ENTRYPOINT
Components::IComponent *entryConfig(libconfig::Setting &config)
{
    return new Components::HealthComponent(config);
}

/**
 * @brief Component name identifier.
 * 
 * This variable defines the name of the component as "Health". It is used by the engine to
 * identify and register the component in the ECS.
 */
LIBRARY_ENTRYPOINT
char const *componentName = "Health";

/**
 * @brief Constructor for HealthComponent using configuration data.
 * 
 * This constructor initializes the HealthComponent by reading the "health" value
 * from the provided libconfig::Setting object. The value is assigned to the health member variable.
 * 
 * @param config A libconfig::Setting object containing the configuration for initializing the component.
 */
Components::HealthComponent::HealthComponent(libconfig::Setting &config)
{
    config.lookupValue("health", health);
}
```

## Version

R-Typeuh component plugins documentation version 0.0.1, 05/10/2024, written by Tugdual de Reviers.
