/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Velocity3d.hpp file
*/

#ifndef VELOCITY_3D_HPP
    #define VELOCITY_3D_HPP

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

namespace Components {
    /**
     * @brief Velocity3d component class in Components.
     * 
     * This class represents the velocity of an entity, storing the x and y components,
     * and provides methods to serialize and deserialize the data.
     */
    class Velocity3d : public AComponent<Velocity3d> {
    public:

        /**
         * @brief Constructor using configuration setting.
         * 
         * Initializes the velocity with the x and y components from the configuration settings.
         * 
         * @param config The configuration settings to use for the component.
         */
        Velocity3d(libconfig::Setting &config) : AComponent("Velocity3d")
        {
            int factor = 0;
            if (!config.lookupValue("x", x)) x = 0;
            if (!config.lookupValue("y", y)) y = 0;
            if (!config.lookupValue("z", z)) z = 0;
            if (!config.lookupValue("factor", factor)) factor = 0;

            floatX = static_cast<float>(x);
            floatY = static_cast<float>(y);
            floatZ = static_cast<float>(z);

            diminishingFactor = static_cast<uint8_t>(factor);
        }

        /**
         * @brief Parameterized constructor for the Velocity3d component.
         * 
         * Initializes the velocity with the provided x and y components.
         * 
         * @param x The X component of the velocity.
         * @param y The Y component of the velocity.
         * @param factor The diminishing factor for the velocity.
         */
        Velocity3d(int32_t x = 0, int32_t y = 0, int32_t z = 0, uint8_t factor = 0) :
            AComponent("Velocity3d"), x(x), y(y), z(z), floatX(x), floatY(y), floatZ(z), diminishingFactor(factor) {
            };

        /**
         * @brief Default destructor for the Velocity3d component.
         */
        ~Velocity3d() = default;

        /**
         * @brief Serializes the velocity components into a byte vector.
         * 
         * Converts the x and y velocity components into network byte order for transmission or storage.
         * 
         * @return A vector of bytes representing the serialized velocity component.
         */
        std::vector<uint8_t> serialize() override {
            __network.x = htonl(x);
            __network.y = htonl(y);
            __network.z = htonl(z);
            __network.diminishingFactor = diminishingFactor;
            return std::vector<uint8_t>(__data, __data + sizeof(__data));
        };

        /**
         * @brief Deserializes the velocity components from the provided byte vector.
         * 
         * Reads the x and y velocity components in network byte order.
         * 
         * @param data A vector of bytes representing the serialized velocity components.
         * @throws std::runtime_error If the data size is invalid.
         */
        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != sizeof(__data))
                throw std::runtime_error("Invalid data size for position component");
            x = ntohl(*reinterpret_cast<int32_t *>(data.data()));
            y = ntohl(*reinterpret_cast<int32_t *>(data.data() + 4));
            z = ntohl(*reinterpret_cast<int32_t *>(data.data() + 8));
            diminishingFactor = data[12];
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
         * @brief Adds the Velocity3d component to an entity.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param args The arguments to pass to the component constructor.
         * 
         * @note This function will add the component to the entity.
         * @note The arguments should be a pair of int32_t values representing the x and y components of the velocity.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) override {
            if (args.size() != 4)
                throw std::runtime_error("Invalid number of arguments for Velocity3d component");

            int32_t x = std::any_cast<int32_t>(args[0]);
            int32_t y = std::any_cast<int32_t>(args[1]);
            int32_t z = std::any_cast<int32_t>(args[2]);
            uint8_t diminishingFactor = std::any_cast<uint8_t>(args[3]);

            engine.getRegistry().componentManager().addComponent<Components::Velocity3d>(to, engine.newComponent<Components::Velocity3d>(x, y, z, diminishingFactor));
        };

        /**
         * @brief Adds the Velocity3d component to an entity.
         * 
         * @param to The entity to add the component to.
         * @param engine The game engine.
         * @param config The configuration settings to use for the component.
         * 
         * @note This function will add the component to the entity.
         * @note The configuration settings should contain the x and y components of the velocity.
         */
        void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) override {
            int xVal = 0, yVal = 0, zVal = 0;
            unsigned int factor = 0;

            if (!config.lookupValue("x", xVal)) {
                std::cerr << "Warning: 'x' not found in config. Using default value: 0\n";
                xVal = 0;
            }

            if (!config.lookupValue("y", yVal)) {
                std::cerr << "Warning: 'y' not found in config. Using default value: 0\n";
                yVal = 0;
            }

            if (!config.lookupValue("z", zVal)) {
                std::cerr << "Warning: 'z' not found in config. Using default value: 0\n";
                zVal = 0;
            }

            if (!config.lookupValue("factor", factor)) {
                std::cerr << "Warning: 'factor' not found in config. Using default value: 0\n";
                factor = 0;
            }

            std::cout << "x: " << xVal << " y: " << yVal << " z: " << zVal << " factor: " << factor << std::endl;

            uint8_t diminishingFactor = static_cast<uint8_t>(factor);

            attachAndUpdateComponent<Components::Velocity3d>(
                engine, to,
                static_cast<int32_t>(xVal),
                static_cast<int32_t>(yVal),
                static_cast<int32_t>(zVal),
                static_cast<uint8_t>(diminishingFactor)
            );
        };

        int32_t x;
        int32_t y;
        int32_t z;
        float floatX;
        float floatY;
        float floatZ;
        float currentVal = 0.0f;
        uint8_t diminishingFactor;

    private:
        union {
            struct {
                int32_t x;
                int32_t y;
                int32_t z;
                uint8_t diminishingFactor;
            } __network;
            uint8_t __data[13];
        };
    };
};

#endif // VELOCITY_3D_HPP
