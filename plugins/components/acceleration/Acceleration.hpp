/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Acceleration.hpp file
*/

#ifndef ACCELERATION_HPP
    #define ACCELERATION_HPP

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

namespace Components {
    /**
     * @brief Acceleration component class in Components.
     * 
     * This class represents the acceleration of an entity,
     * storing four data: forward, backward, right, and left acceleration values,
     * and provides methods to serialize and deserialize the data.
     */
    class Acceleration : public AComponent<Acceleration> {
    public:

        /**
         * @brief Constructor using configuration setting.
         * 
         * Initializes the acceleration with the four components from the configuration settings.
         * forward, backward, right, and left.
         * Each corresponding to the acceleration in the direction.
         * 
         * @param config The configuration settings to use for the component.
         */
        Acceleration(libconfig::Setting &config) : AComponent("Acceleration")
        {
            if (!config.lookupValue("forward", forward)) forward = 2;
            if (!config.lookupValue("backward", backward)) backward = 1;
            if (!config.lookupValue("right", right)) right = 1;
            if (!config.lookupValue("left", left)) left = 1;
        }

        /**
         * @brief Parameterized constructor for the Acceleration component.
         * 
         * Initializes the acceleration with the provided forward, backward, right, and left components.
         * 
         * @param forward The forward acceleration value.
         * @param backward The backward acceleration value.
         * @param right The right acceleration value.
         * @param left The left acceleration value.
         */
        Acceleration(int32_t forward = 2, int32_t backward = 1, int32_t right = 1, int32_t left = 1) :
            AComponent("Acceleration"), forward(forward), backward(backward), right(right), left(left) {
            };

        /**
         * @brief Default destructor for the Acceleration component.
         */
        ~Acceleration() = default;

        /**
         * @brief Serializes the acceleration components into a byte vector.
         * 
         * Converts the forward, backward, right, and left acceleration components
         * into network byte order for transmission or storage.
         * 
         * @return A vector of bytes representing the serialized acceleration component.
         */
        std::vector<uint8_t> serialize() override {
            __network.forward = htonl(forward);
            __network.backward = htonl(backward);
            __network.right = htonl(right);
            __network.left = htonl(left);
            return std::vector<uint8_t>(__data, __data + sizeof(__data));
        };

        /**
         * @brief Deserializes the acceleration component from the provided byte vector.
         * 
         * Reads the forward, backward, right, and left acceleration components in network byte order.
         * 
         * @param data A vector of bytes representing the serialized acceleration component.
         * @throws std::runtime_error If the data size is invalid.
         */
        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != sizeof(__data))
                throw std::runtime_error("Invalid data size for position component");

            forward = ntohl(*reinterpret_cast<int32_t *>(data.data()));
            backward = ntohl(*reinterpret_cast<int32_t *>(data.data() + 4));
            right = ntohl(*reinterpret_cast<int32_t *>(data.data() + 8));
            left = ntohl(*reinterpret_cast<int32_t *>(data.data() + 12));
        };

        /**
         * @brief Gets the size of the serialized data.
         * 
         * @return The size of the data, in bytes.
         */
        size_t getSize() const override {
            return sizeof(__data);
        };

        /**
         * @brief Adds the Acceleration component to an entity.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param args The arguments to pass to the component constructor.
         * 
         * @note This function will add the component to the entity.
         * @note The arguments should be the forward, backward, right, and left acceleration values, all int32_t.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() != 4)
                throw std::runtime_error("Invalid number of arguments for Acceleration component");

            int32_t forward = std::any_cast<int32_t>(args[0]);
            int32_t backward = std::any_cast<int32_t>(args[1]);
            int32_t right = std::any_cast<int32_t>(args[2]);
            int32_t left = std::any_cast<int32_t>(args[3]);

            engine.getRegistry().componentManager().addComponent<Components::Acceleration>(to, engine.newComponent<Components::Acceleration>(forward, backward, right, left));
        };

        /**
         * @brief Adds the Acceleration component to an entity.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param config The configuration settings to use for the component.
         * 
         * @note This function will add the component to the entity.
         * @note The configuration settings should contain the forward, backward, right, and left acceleration values.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            int forwardVal = 0, backwardVal = 0, rightVal = 0, leftVal = 0;

            if (!config.lookupValue("forward", forwardVal)) {
                std::cerr << "Warning: 'forward' not found in config. Using default value: 0\n";
                forwardVal = 2;
            }

            if (!config.lookupValue("backward", backwardVal)) {
                std::cerr << "Warning: 'backward' not found in config. Using default value: 0\n";
                backwardVal = 1;
            }

            if (!config.lookupValue("right", rightVal)) {
                std::cerr << "Warning: 'right' not found in config. Using default value: 0\n";
                rightVal = 1;
            }

            if (!config.lookupValue("left", leftVal)) {
                std::cerr << "Warning: 'left' not found in config. Using default value: 0\n";
                leftVal = 1;
            }

            std::cout << "forward: " << forwardVal << " backward: " << backwardVal << " right: " << rightVal << " left: " << leftVal << std::endl;

            std::unique_ptr<Components::Acceleration> pos = engine.newComponent<Components::Acceleration>(static_cast<int32_t>(forwardVal), static_cast<int32_t>(backwardVal), static_cast<int32_t>(rightVal), static_cast<int32_t>(leftVal));
            engine.getRegistry().componentManager().addComponent<Components::Acceleration>(to, std::move(pos));
            std::cout << std::endl;
        };

        int32_t forward;
        int32_t backward;
        int32_t right;
        int32_t left;

    private:
        union {
            struct {
                int32_t forward;
                int32_t backward;
                int32_t right;
                int32_t left;
            } __network;
            uint8_t __data[16];
        };
    };
};

#endif // ACCELERATION_HPP
