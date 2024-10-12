/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Visible.hpp file
*/

#ifndef VISIBLE_HPP
#define VISIBLE_HPP

    #include "plugins/components/AComponent.hpp"
    #include "GameEngine/GameEngine.hpp"
    #include "components/AComponent.hpp"
     #ifdef _WIN32
        #include <windows.h>
        #pragma comment(lib, "ws2_32.lib")
    #else
        #include <arpa/inet.h>
    #endif
    #include <vector>
    #include <stdexcept>
    #include <libconfig.h++>

/**
 * @namespace Components
 * @brief Namespace for all ECS Components.
 */
namespace Components {

/**
 * @class Visible
 * @brief Visible component class in Components.
 * 
 * The Visible component holds a boolean value (`isVisible`) that indicates whether
 * an entity should be displayed in the game. This component can be serialized and
 * deserialized for network transmission.
 */
class Visible : public AComponent {
public:
    /**
     * @brief Default constructor for the Visible component.
     * 
     * Initializes `isVisible` to false by default, indicating that the entity is invisible.
     */
    Visible(bool isVisible = false) : AComponent("Visible"), isVisible(isVisible) {}

    /**
     * @brief Constructor for the Visible component that initializes from a configuration setting.
     * 
     * @param config The configuration setting to extract the initial visibility state from.
     * 
     * @throws std::invalid_argument If the 'isVisible' key is not found in the configuration.
     */
    Visible(libconfig::Setting &config) : AComponent(std::string("Visible")) {
        if (!config.lookupValue("isVisible", isVisible))
            throw std::runtime_error("Missing 'isVisible' setting for Visible component");
    }

    /**
     * @brief Serializes the visibility state.
     * 
     * Converts the `isVisible` boolean into a single byte that can be transmitted
     * or stored.
     * 
     * @return A vector of bytes representing the serialized visibility state.
     */
    std::vector<uint8_t> serialize() override {
        return { static_cast<uint8_t>(isVisible) };
    }

    /**
     * @brief Deserializes the visibility state from a byte vector.
     * 
     * Reads the visibility state from a byte vector. This function expects a vector of size 1,
     * containing a single byte that represents the visibility state (`true` or `false`).
     * 
     * @param data A vector of bytes representing the serialized visibility state.
     * @throws std::runtime_error If the size of the data is not exactly 1 byte.
     */
    void deserialize(std::vector<uint8_t> &data) override {
        if (data.size() != 1)
            throw std::runtime_error("Invalid data size for Visible component");

        isVisible = static_cast<bool>(data[0]);
    }

    /**
     * @brief Gets the size of the serialized component.
     * 
     * The size of the Visible component when serialized is always 1 byte, as it only
     * stores a single boolean value.
     * 
     * @return The size of the serialized component, in bytes.
     */
    size_t getSize() const override {
        return (sizeof(__data));
    }

    /**
     * @brief Adds the Visible component to an entity.
     * 
     * @param to The entity to add the component to.
     * @param engine The game engine.
     * @param args The arguments to pass to the component constructor.
     * 
     * @note This function will add the Visible component to the entity.
     * @note The arguments should be a boolean value representing visibility.
     */
    void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
        if (args.size() != 1)
            throw std::runtime_error("Invalid number of arguments for Visible component");
        
        bool visibility = std::any_cast<bool>(args[0]);
        engine.getRegistry().componentManager().addComponent<Components::Visible>(to, engine.newComponent<Components::Visible>(visibility));
    }

    /**
     * @brief Adds the Visible component to an entity from a configuration setting.
     * 
     * @param to The entity to add the component to.
     * @param engine The game engine.
     * @param config The configuration setting to extract the component data from.
     * 
     * @note This function will add the Visible component to the entity.
     * @note The configuration setting should contain the key 'isVisible'.
     */
    void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
        bool isVisibleConfig = false;

        if (!config.lookupValue("isVisible", isVisibleConfig)) {
            throw std::invalid_argument("Failed to retrieve 'isVisible' value from config");
        }

        std::cout << "isVisible: " << isVisibleConfig << std::endl;

        std::unique_ptr<Components::Visible> visibilityComponent = engine.newComponent<Components::Visible>(isVisibleConfig);
        engine.getRegistry().componentManager().addComponent<Components::Visible>(to, std::move(visibilityComponent));
        std::cout << std::endl;
    }

    /**
     * @brief Indicates whether the entity is visible or not.
     */
    bool isVisible;
    char const *componentName;

private:
    /**
     * @brief Network packet representation for the Visible component.
     */
    union {
        struct {
            uint8_t isVisible;
        } __network;
        uint8_t __data[1];
    };
};

}; // namespace Components

#endif // VISIBLE_HPP
