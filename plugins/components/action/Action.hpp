/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Action.hpp file
*/

#ifndef ACTION_HPP
    #define ACTION_HPP

    #include "GameEngine/GameEngine.hpp"
    #include "components/AComponent.hpp"
    #include "utils/EntityActions.hpp"

    #ifdef _WIN32
        #include <windows.h>
        #pragma comment(lib, "ws2_32.lib")
    #else
        #include <arpa/inet.h>
    #endif
    #include <vector>
    #include <stdexcept>
    #include <libconfig.h++>
    #include <array>
    #include <map>

namespace Components {
    /**
     * @brief Action component class in Components.
     * 
     * This class is a component that gives details about the current action of an entity.
     * 
     * @note Actions are defined in the Actions.hpp file.
     * @note It provides methods to serialize and deserialize the action data for network transmission or storage.
     */
    class ActionComponent : public AComponent<ActionComponent> {
    public:

        /**
         * @brief Constructor of the Action component.
         * 
         * Initializes the Action component.
         */
        ActionComponent() :
            AComponent("Action") {};

        /**
         * @brief Constructor with configuration settings.
         * 
         * This constructor initializes the Action component but uses no values from the configuration.
         * 
         * @param config A configuration object to extract values for controllable inputs. Not used in this component
         */
        ActionComponent(libconfig::Setting &) :
            AComponent("Action")
        {
        }

        /**
         * @brief Destructor for the Action component.
         */
        ~ActionComponent() = default;

        /**
         * @brief Serializes the component data.
         * 
         * Converts the action into network byte order for transmission or storage.
         * 
         * @return A vector of bytes representing the serialized action.
         */
        std::vector<uint8_t> serialize() override {
            return {static_cast<uint8_t>(action)};
        }

        /**
         * @brief Deserializes the component data from a byte vector.
         * 
         * @param data The byte vector to deserialize into the component.
         * 
         * @throw std::runtime_error If the data size is invalid.
         * 
         * @note The data should contain a single byte representing the current action state.
         */
        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != 1)
                throw std::runtime_error("Invalid data size for Action component");

            action = static_cast<enum EntityAction>(data[0]);
        }

        /**
         * @brief Gets the size of the serialized data.
         * 
         * @return The size of the data, in bytes.
         */
        size_t getSize() const override { return sizeof(action); };

        /**
         * @brief Adds the Action component to an entity.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param args The arguments to pass to the component constructor (can be booleans for movement/actions).
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any>) override {
            std::unique_ptr<Components::ActionComponent> ctrl = engine.newComponent<Components::ActionComponent>();
            engine
                .getRegistry()
                .componentManager()
                .addComponent<Components::ActionComponent>(to, std::move(ctrl));
        };

        /**
         * @brief Adds the Action component to an entity from a configuration setting.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param config The configuration setting to extract the component data from.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &) override {
            std::unique_ptr<Components::ActionComponent> ctrl = engine.newComponent<Components::ActionComponent>();

            Components::IComponent *comp = ctrl.get();
            engine
                .getRegistry()
                .componentManager()
                .addComponent<Components::ActionComponent>(to, std::move(ctrl));
            engine.updateComponent(to, comp->getId(), comp->serialize());
        };

        enum EntityAction action; // Holds the current action state

    };
};

#endif // ACTION_HPP
