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
     * @brief Ai component class in Components.
     * 
     * This class represents a rectangle Ai for an entity, storing the width and height of the Ai.
     * It provides methods to serialize and deserialize the Ai data for network transmission or storage.
     */
    class Ai : public AComponent<Ai> {
    public:
        /**
         * @brief Default constructor
         * 
         * Initializes the rectangle Ai with default width and height of 0.
         */
        Ai(uint32_t behavior = 0) : AComponent(std::string("Ai")), behavior(behavior) {};

        /**
         * @brief Constructor using configuration.
         * 
         * Initializes the rectangle Ai based on config settings, with default values
         * if lookupValue fails.
         */
        Ai(libconfig::Setting &config) : AComponent(std::string("Ai")) {
            if (!config.lookupValue("behavior", behavior)) {
                behavior = 0;
            }
        }        

        /**
         * @brief Serialize the Ai data
         * 
         * Serializes the width and height into a byte vector for transmission or storage.
         * 
         * @return std::vector<uint8_t> Serialized data.
         */
        std::vector<uint8_t> serialize() override {
            __network.behavior = htonl(behavior);
            return std::vector<uint8_t>(__data, __data + sizeof(__data));
        }

        /**
         * @brief Deserialize the Ai data
         * 
         * Deserializes the width and height from the provided byte vector.
         * 
         * @param data The byte vector containing serialized data.
         * @throws std::runtime_error If the data size is invalid.
         */
        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != getSize())
                throw std::runtime_error("Invalid data size for Ai component");

            behavior = ntohl(*reinterpret_cast<uint32_t *>(data.data()));
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
         * @brief Adds the Ai component to an entity.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param args The arguments to pass to the component constructor.
         * 
         * @note The arguments should be a tuple containing the width and height.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() != 1)
                throw std::runtime_error("Invalid number of arguments for Ai component");

            uint32_t behavior = std::any_cast<uint32_t>(args[0]);

            auto Ai = engine.newComponent<Components::Ai>(behavior);
            engine.getRegistry().componentManager().addComponent<Components::Ai>(to, std::move(Ai));
        };

        /**
         * @brief Adds the Ai component to an entity from a configuration setting.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param config The configuration setting to extract the component data from.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            int behavior = 0; 

            if (!config.lookupValue("behavior", behavior)) {
                std::cerr << "Warning: 'behavior' not found in config. Using default value: 1\n";
                behavior = 0;
            }

            std::cout << "behavior: " << behavior << std::endl;

            std::unique_ptr<Components::Ai> Ai = engine.newComponent<Components::Ai>(static_cast<uint32_t>(behavior));
            engine.getRegistry().componentManager().addComponent<Components::Ai>(to, std::move(Ai));
            std::cout << std::endl;
        }

        uint32_t behavior;

    private:
        union {
            struct {
                uint32_t behavior;
            } __network;
            uint8_t __data[4];
        };
    };
};

#endif // AI_HPP
