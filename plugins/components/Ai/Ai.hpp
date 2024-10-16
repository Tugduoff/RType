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
