/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Score.hpp file
*/

#ifndef SCORE_HPP
    #define SCORE_HPP

    #include "plugins/components/AComponent.hpp"
    #include "GameEngine/GameEngine.hpp"
    #include "components/AComponent.hpp"
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

namespace Components {

    /**
     * @brief Score component for ECS
     * 
     * This class represents the score of an entity.
     * It provides methods for serialization and deserialization to allow for network transmission or storage.
     */
    class Score : public AComponent<Score> {
    public:

        /**
         * @brief Default constructor for Score component.
         * 
         * Initializes score to default value of 0.
         */
        Score(uint32_t value = 0) :
            AComponent(std::string("Score")), value(value) {};

        /**
         * @brief Constructs a Score component with values from the configuration.
         *
         * This constructor initializes the Score component by attempting to read 
         * `value` and `maxScore` from the provided configuration. If these 
         * values are not found in the config, default values of 100 are used.
         *
         * @param config The configuration settings to retrieve score values from.
         *
         * @note If the `value` or `maxScore` values are missing from the 
         *       configuration, they will be defaulted to 100.
         */
        Score(libconfig::Setting &config) : AComponent(std::string("Score")) {
            if (!config.lookupValue("value", value))
                value = 0;
        };

        // operator =
        int operator=(const Score &score) {
            value = score.value;
            return value;
        }

        /**
         * @brief Serializes the score values into a byte vector.
         * 
         * Converts current and max score into network byte order for transmission or storage.
         * 
         * @return A vector of bytes representing the serialized score component.
         */
        std::vector<uint8_t> serialize() override {
            __network.value = htonl(value);
            return std::vector<uint8_t>(__data, __data + sizeof(__data));
        };

        /**
         * @brief Deserializes the score component from a byte vector.
         * 
         * Reads the current and max score values from network byte order.
         * 
         * @param data A vector of bytes representing the serialized score component.
         * @throws std::runtime_error If the data size is invalid.
         */
        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != sizeof(__data))
                throw std::runtime_error("Invalid data size for Score component");

            value = ntohl(*reinterpret_cast<uint32_t *>(data.data()));
        };

        /**
         * @brief Get the size of the serialized data.
         * 
         * @return The size of the data, in bytes.
         */
        size_t getSize() const override {
            return sizeof(__data);
        };

        /**
         * @brief Adds the Score component to an entity.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param args The arguments to pass to the component constructor.
         * 
         * @note This function will add the Score component to the entity.
         * @note The arguments should be a uint32_t value: value.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() != 1)
                throw std::runtime_error("Invalid number of arguments for Score component");

            uint32_t value = std::any_cast<uint32_t>(args[0]);

            engine.getRegistry().componentManager().addComponent<Components::Score>(to, engine.newComponent<Components::Score>(value));
        };

        /**
         * @brief Adds the Score component to an entity from a configuration setting.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param config The configuration setting to extract the component data from.
         * 
         * @note This function will add the Score component to the entity.
         * @note The configuration setting should contain the key 'value'.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            int value = 0;

            if (!config.lookupValue("value", value)) {
                std::cerr << "Warning: 'value' not found in config. Using default value: 1.0f\n";
                value = 0;
            }

            attachAndUpdateComponent<Components::Score>(
                engine, to,
                static_cast<uint32_t>(value)
            );
        };

        uint32_t value;
        char const *componentName;

    private:
        union {
            struct {
                uint32_t value;
            } __network;
            uint8_t __data[4];
        };
    };
};

#endif // SCORE_HPP
