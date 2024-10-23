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
        Ai(uint32_t behavior = 0, uint32_t animLength = 0, uint32_t animSpeed = 0) : AComponent(std::string("Ai")), behavior(behavior), animLength(animLength), animSpeed(animSpeed) {};

        /**
         * @brief Constructor using configuration settings.
         * 
         * Initializes the AI component based on values retrieved from a configuration.
         * If the "behavior" setting is missing, it will default to 0.
         * 
         * @param config The libconfig::Setting containing the configuration data.
         */
        Ai(libconfig::Setting &config) : AComponent(std::string("Ai")) {
            if (!config.lookupValue("behavior", behavior)) {
                behavior = 0;
            }
            if (!config.lookupValue("animLength", animLength)) {
                animLength = 0;
            }
            if (!config.lookupValue("animSpeed", animSpeed)) {
                animSpeed = 0;
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
            __network.behavior = htonl(behavior);
            __network.animLength = htonl(animLength);
            __network.animSpeed = htonl(animSpeed);
            return std::vector<uint8_t>(__data, __data + sizeof(__data));
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

            behavior = ntohl(*reinterpret_cast<uint32_t *>(data.data()));
            animLength = ntohl(*reinterpret_cast<uint32_t *>(data.data() + 4));
            animSpeed = ntohl(*reinterpret_cast<uint32_t *>(data.data() + 8));
        }

        /**
         * @brief Get the fixed size of the serialized data
         * 
         * @return size_t Size in bytes
         */
        size_t getSize() const override {
            return sizeof(__data);
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
            if (args.size() != 1)
                throw std::runtime_error("Invalid number of arguments for Ai component");

            uint32_t behavior = std::any_cast<uint32_t>(args[0]);
            uint32_t animLength = std::any_cast<uint32_t>(args[1]);
            uint32_t animSpeed = std::any_cast<uint32_t>(args[2]);

            auto Ai = engine.newComponent<Components::Ai>(behavior, animLength, animSpeed);
            engine.getRegistry().componentManager().addComponent<Components::Ai>(to, std::move(Ai));
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
            int animLength = 0;
            int animSpeed = 0;

            if (!config.lookupValue("behavior", behavior)) {
                std::cerr << "Warning: 'behavior' not found in config. Using default value: 1\n";
                behavior = 0;
            }
            if (!config.lookupValue("animLength", animLength)) {
                std::cerr << "Warning: 'animLength' not found in config. Using default value: 1\n";
                animLength = 0;
            }
            if (!config.lookupValue("animSpeed", animSpeed)) {
                std::cerr << "Warning: 'animSpeed' not found in config. Using default value: 1\n";
                animSpeed = 0;
            }


            std::unique_ptr<Components::Ai> Ai = engine.newComponent<Components::Ai>(static_cast<uint32_t>(behavior), static_cast<uint32_t>(animLength), static_cast<uint32_t>(animSpeed));
            engine.getRegistry().componentManager().addComponent<Components::Ai>(to, std::move(Ai));
            std::cout << std::endl;
        }

        uint32_t behavior;
        uint32_t animLength;
        uint32_t animSpeed;
        std::string animSpriteSheet;

    private:
        union {
            struct {
                uint32_t behavior;
                uint32_t animLength;
                uint32_t animSpeed;
            } __network;
            uint8_t __data[12];
        };
    };
};

#endif // AI_HPP
