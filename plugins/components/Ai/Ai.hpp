/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Ai.hpp file
*/

#ifndef AI_HPP
    #define AI_HPP

    #include "plugins/components/AComponent.hpp"
    #include "GameEngine/GameEngine.hpp"
    #ifdef _WIN32
        #include <windows.h>
        #pragma comment(lib, "ws2_32.lib")
    #else
        #include <arpa/inet.h>
    #endif
    #include <vector>
    #include <stdexcept>
    #include <cstdint>
    #include <libconfig.h++>

namespace Components {

    enum class BehaviorId : uint8_t {
        NOTHING = 0,
        Y_AXIS_LOOP,
        Y_ZIG_ZAG_1,
        Y_ZIG_ZAG_2,
        Y_ZIG_ZAG_3,
        Y_ZIG_ZAG_4,
        X_AXIS_LOOP,
        X_ZIG_ZAG_1,
        X_ZIG_ZAG_2,
        X_ZIG_ZAG_3,
        X_ZIG_ZAG_4,
        UNKNOWN = 255 // Handle an invalid behavior
    };

    /**
     * @brief AI component class in Components.
     * 
     * This class represents an AI behavior for an entity, storing the behavior value.
     * It provides methods to serialize and deserialize AI behavior data for network transmission or storage.
     */
    class Ai : public AComponent<Ai> {
    public:
        /**
         * @brief Constructor to initialize AI with a behavior.
         * 
         * Initializes the AI component with a given behavior value.
         * 
         * @param behavior The AI behavior identifier (default is 0).
         */
        Ai(const BehaviorId &behavior = BehaviorId::Y_AXIS_LOOP) : AComponent(std::string("Ai")), behavior(behavior) {};

        /**
         * @brief Constructor using configuration settings.
         * 
         * Initializes the AI component based on values retrieved from a configuration.
         * If the "behavior" setting is missing, it will default to 0.
         * 
         * @param config The libconfig::Setting containing the configuration data.
         */
        Ai(libconfig::Setting &config) : AComponent(std::string("Ai"))
        {
            int Id = 0;

            if (!config.lookupValue("behavior", Id)) {
                behavior = BehaviorId::Y_AXIS_LOOP;
            } else {
                behavior = static_cast<BehaviorId>(Id);
            }
        }        

        /**
         * @brief Serialize the AI behavior.
         * 
         * Serializes the behavior into a byte vector for transmission or storage.
         * 
         * @return std::vector<uint8_t> Serialized behavior data.
         */
        std::vector<uint8_t> serialize() override {
            return {static_cast<uint8_t>(behavior)};
        }

        /**
         * @brief Deserialize the AI behavior from data.
         * 
         * Deserializes the behavior value from a byte vector.
         * 
         * @param data The byte vector containing serialized behavior data.
         * @throws std::runtime_error If the data size is incorrect.
         */
        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != getSize())
                throw std::runtime_error("Invalid data size for Ai component");

            behavior = static_cast<BehaviorId>(data[0]);
        }

        /**
         * @brief Get the fixed size of the serialized data
         * 
         * @return size_t Size in bytes
         */
        size_t getSize() const override {
            return sizeof(uint8_t);
        }

        /**
         * @brief Adds the AI component to an entity.
         * 
         * Adds an AI component to the specified entity with the given behavior value.
         * 
         * @param to The entity to add the AI component to.
         * @param engine The game engine managing the components.
         * @param args A vector containing the behavior value (as a uint32_t).
         * 
         * @throws std::runtime_error If the arguments are invalid.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() < 1)
                throw std::runtime_error("Invalid number of arguments for Ai component");

            BehaviorId behavior = std::any_cast<BehaviorId>(args[0]);
            std::unique_ptr<Components::Ai> aiComponent =
                engine.newComponent<Components::Ai>(behavior);

            engine.getRegistry().componentManager().addComponent(to, std::move(aiComponent));
        };

        /**
         * @brief Adds the AI component to an entity using a configuration.
         * 
         * Adds an AI component to the specified entity based on the configuration settings.
         * 
         * @param to The entity to add the AI component to.
         * @param engine The game engine managing the components.
         * @param config The configuration settings for initializing the component.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            int behavior = 0; 

            if (!config.lookupValue("behavior", behavior)) {
                std::cerr << "Warning: 'behavior' not found in config. Using default value: 1\n";
                behavior = 0;
            }
            BehaviorId Id = static_cast<BehaviorId>(behavior);
            std::unique_ptr<Components::Ai> aiComponents =
                engine.newComponent<Components::Ai>(Id);

            engine.getRegistry().componentManager().addComponent<Components::Ai>(to, std::move(aiComponents));
        }

        BehaviorId behavior;

    private:
    };
};

#endif // AI_HPP
