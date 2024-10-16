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
                behavior = 1;
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
