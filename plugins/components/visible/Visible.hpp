/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Visible.hpp file
*/

#ifndef VISIBLE_HPP
    #define VISIBLE_HPP

    #include "plugins/components/AComponent.hpp"
    #include <vector>
    #include <stdexcept>

/**
 * @namespace Components
 * @brief Namespace for all ECS components.
 */
namespace Components {

    /**
     * @class Visible
     * @brief Component that determines whether an entity should be rendered.
     * 
     * The Visible component holds a boolean value (`isVisible`) that indicates whether
     * an entity should be displayed in the game. This component can be serialized and
     * deserialized for network transmission.
     */
    class Visible : public AComponent {
    public:

        /**
         * @brief Default constructor for Visible component.
         * 
         * Initializes `isVisible` to true by default, indicating that the entity is visible.
         */
        Visible() : isVisible(true) {};

        /**
         * @brief Parameterized constructor for Visible component.
         * 
         * @param isVisible The initial visibility state of the entity.
         */
        Visible(bool isVisible) : isVisible(isVisible) {};

        /**
         * @brief Serializes the visibility state.
         * 
         * Converts the `isVisible` boolean into a single byte that can be transmitted
         * or stored. This is useful for saving or sending the visibility state over
         * a network.
         * 
         * @return A vector of bytes representing the serialized visibility state.
         */
        std::vector<uint8_t> serialize() override {
            return { static_cast<uint8_t>(isVisible) };
        };

        /**
         * @brief Deserializes the visibility state from a byte vector.
         * 
         * Reads the visibility state from a byte vector. This function expects a vector of size 1,
         * containing a single byte that represents the visibility state (`true` or `false`).
         * 
         * @param data A vector of bytes representing the serialized visibility state.
         * @throws std::runtime_error If the size of the data is not exactly 1 byte.
         */
        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != 1)
                throw std::runtime_error("Invalid data size for Visible component");

            isVisible = static_cast<bool>(data[0]);
        };

        /**
         * @brief Gets the size of the serialized component.
         * 
         * The size of the Visible component when serialized is always 1 byte, as it only
         * stores a single boolean value.
         * 
         * @return The size of the serialized component, in bytes.
         */
        size_t getSize() const override {
            return 1;
        };

        /**
         * @brief Indicates whether the entity is visible or not.
         */
        bool isVisible;

    private:
        union {
            struct {
                uint8_t isVisible;
            } __network;
            uint8_t __data[1];
        };
    };
};

#endif // VISIBLE_HPP
