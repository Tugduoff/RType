/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Controllable.hpp file
*/

#ifndef CONTROLLABLE_HPP
    #define CONTROLLABLE_HPP

    #include "plugins/components/AComponent.hpp"
    #include "GameEngine/GameEngine.hpp"
    #include "components/AComponent.hpp"
    #include "utils/Keys.hpp"
    #include "utils/Actions.hpp"
    #include "utils/ComponentUtils.hpp"
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
     * @brief Controllable component class in Components.
     * 
     * This class is a component that can be added to an entity to make it controllable by user inputs.
     * It holds a list of key bindings for movement and up to 10 different actions.
     * 
     * Example configuration:
     * args = {
     *    FORWARD = true,
     *    BACKWARD = false,
     *    LEFT = false,
     *    RIGHT = true,
     *    ACTION1 = true,
     *    ACTION2 = false,
     *    ...
     * }
     */
    class Controllable : public AComponent<Controllable> {
    public:

        /**
         * @brief Constructor with key bindings.
         * 
         * @param keyBindings A map of key bindings for movement and actions.
         * 
         * Initializes the Controllable component with the provided key bindings.
         */
        Controllable(std::map<enum Action, enum Key> keyBindings = {
                {Action::FORWARD, Key::Z},
                {Action::BACKWARD, Key::S},
                {Action::LEFT, Key::Q},
                {Action::RIGHT, Key::D},
                {Action::ACTION1, Key::LEFT_CLICK},
                {Action::ACTION2, Key::RIGHT_CLICK},
                {Action::ACTION3, Key::MIDDLE_CLICK},
                {Action::ACTION4, Key::NUM_0},
                {Action::ACTION5, Key::NUM_1},
                {Action::ACTION6, Key::NUM_2},
                {Action::ACTION7, Key::NUM_3},
                {Action::ACTION8, Key::NUM_4},
                {Action::ACTION9, Key::NUM_5},
                {Action::ACTION10, Key::NUM_6}
            }) :
            AComponent("Controllable"),
            inputs{false},
            actions{false},
            keyBindings(keyBindings) {};

        /**
         * @brief Constructor with configuration settings.
         * 
         * This constructor initializes the Controllable component using values from the configuration.
         * 
         * @param config A configuration object to extract values for controllable inputs.
         */
        Controllable(libconfig::Setting &config) :
            AComponent("Controllable"), inputs{false}, actions{false},
            keyBindings({
                {Action::FORWARD, Key::Z},
                {Action::BACKWARD, Key::S},
                {Action::LEFT, Key::Q},
                {Action::RIGHT, Key::D},
                {Action::ACTION1, Key::LEFT_CLICK},
                {Action::ACTION2, Key::RIGHT_CLICK},
                {Action::ACTION3, Key::MIDDLE_CLICK},
                {Action::ACTION4, Key::NUM_0},
                {Action::ACTION5, Key::NUM_1},
                {Action::ACTION6, Key::NUM_2},
                {Action::ACTION7, Key::NUM_3},
                {Action::ACTION8, Key::NUM_4},
                {Action::ACTION9, Key::NUM_5},
                {Action::ACTION10, Key::NUM_6}
            })
        {
            std::string forward, backward, left, right;

            if (!config.lookupValue("FORWARD", forward)) forward = "Z";
            if (!config.lookupValue("BACKWARD", backward)) backward = "S";
            if (!config.lookupValue("LEFT", left)) left = "Q";
            if (!config.lookupValue("RIGHT", right)) right = "D";

            keyBindings[Action::FORWARD] = strToKey.at(forward);
            keyBindings[Action::BACKWARD] = strToKey.at(backward);
            keyBindings[Action::LEFT] = strToKey.at(left);
            keyBindings[Action::RIGHT] = strToKey.at(right);

            for (int i = 0; i < 10; i++) {
                std::string actionKey = "ACTION" + std::to_string(i + 1);
                std::string actionValue;

                if (!config.lookupValue(actionKey, actionValue))
                    continue;

                Action action = strToAction.at(actionKey);
                std::cout << "Action: " << action << " Key: " << actionValue << std::endl;
                keyBindings[action] = strToKey.at(actionValue);
            }
        }

        /**
         * @brief Destructor for the Controllable component.
         */
        ~Controllable() = default;

        /**
         * @brief Serializes the input states into a byte vector.
         * 
         * Converts the inputs (movement directions and actions) into network byte order for transmission or storage.
         * 
         * @return A vector of bytes representing the serialized input states.
         */
        std::vector<uint8_t> serialize() override {
            // Pack inputs and actions into the data array
            __network.inputs = inputs;
            __network.actions = actions;
            return std::vector<uint8_t>(__data, __data + sizeof(__data));
        };

        /**
         * @brief Deserializes the input states from the provided byte vector.
         * 
         * Reads the inputs (forward, backward, left, right) and actions in network byte order.
         * 
         * @param data A vector of bytes representing the serialized input states.
         * @throws std::runtime_error If the data size is invalid.
         */
        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != sizeof(__data))
                throw std::runtime_error("Invalid data size for Controllable component");
            __network.inputs = *reinterpret_cast<std::array<bool, 4> *>(data.data());
            __network.actions = *reinterpret_cast<std::array<bool, 10> *>(data.data() + 4);
        };

        /**
         * @brief Gets the size of the serialized data.
         * 
         * @return The size of the data, in bytes.
         */
        size_t getSize() const override { return sizeof(__data); };

        /**
         * @brief Adds the Controllable component to an entity.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param args The arguments to pass to the component constructor (can be booleans for movement/actions).
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() != 1)
                throw std::runtime_error("Invalid number of arguments for Controllable component");

            std::map<enum Action, enum Key> newKeyBindings = std::any_cast<std::map<enum Action, enum Key>>(args[0]);
            std::unique_ptr<Components::Controllable> ctrl = engine.newComponent<Components::Controllable>(newKeyBindings);
            engine
                .getRegistry()
                .componentManager()
                .addComponent<Components::Controllable>(to, std::move(ctrl));
        };

        /**
         * @brief Adds the Controllable component to an entity from a configuration setting.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param config The configuration setting to extract the component data from.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            std::string forward, backward, left, right;

            if (!config.lookupValue("FORWARD", forward)) forward = "Z";
            if (!config.lookupValue("BACKWARD", backward)) backward = "S";
            if (!config.lookupValue("LEFT", left)) left = "Q";
            if (!config.lookupValue("RIGHT", right)) right = "D";

            std::map<enum Action, enum Key> newKeyBindings = {
                {Action::FORWARD, Key::Z},
                {Action::BACKWARD, Key::S},
                {Action::LEFT, Key::Q},
                {Action::RIGHT, Key::D},
                {Action::ACTION1, Key::LEFT_CLICK},
                {Action::ACTION2, Key::RIGHT_CLICK},
                {Action::ACTION3, Key::MIDDLE_CLICK},
                {Action::ACTION4, Key::NUM_0},
                {Action::ACTION5, Key::NUM_1},
                {Action::ACTION6, Key::NUM_2},
                {Action::ACTION7, Key::NUM_3},
                {Action::ACTION8, Key::NUM_4},
                {Action::ACTION9, Key::NUM_5},
                {Action::ACTION10, Key::NUM_6}
            };

            newKeyBindings[Action::FORWARD] = strToKey.at(forward);
            newKeyBindings[Action::BACKWARD] = strToKey.at(backward);
            newKeyBindings[Action::LEFT] = strToKey.at(left);
            newKeyBindings[Action::RIGHT] = strToKey.at(right);

            std::cerr << "FORWARD: " << newKeyBindings[Action::FORWARD] << std::endl;
            std::cerr << "BACKWARD: " << newKeyBindings[Action::BACKWARD] << std::endl;
            std::cerr << "LEFT: " << newKeyBindings[Action::LEFT] << std::endl;
            std::cerr << "RIGHT: " << newKeyBindings[Action::RIGHT] << std::endl;

            for (int i = 0; i < 10; i++) {
                std::string actionKey = "ACTION" + std::to_string(i + 1);
                std::string actionValue;
                Action action = strToAction.at(actionKey);

                if (!config.lookupValue(actionKey, actionValue)) {
                    newKeyBindings[action] = Key::UNKNOWN;
                    std::cerr << actionKey << ": UNKNOWN" << std::endl;
                    continue;
                }

                newKeyBindings[action] = strToKey.at(actionValue);
                std::cerr << actionKey << ": " << newKeyBindings[action] << std::endl;
            }

            attachAndUpdateComponent<Components::Controllable>(
                engine, to,
                newKeyBindings
            );
        };

        std::array<bool, 4> inputs;   // { forward, backward, left, right }
        std::array<bool, 10> actions; // Actions from 1 to 10
        std::map<enum Action, enum Key> keyBindings; // Key bindings for movement and actions

    private:
        union {
            struct {
                std::array<bool, 4> inputs;
                std::array<bool, 10> actions;
            } __network;
            uint8_t __data[14];  // Holds the serialized input and action states
        };
    };
};

#endif // CONTROLLABLE_HPP
