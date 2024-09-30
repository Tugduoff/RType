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

namespace Components {

    class Visible : public AComponent {
    public:

        Visible() : isVisible(true) {};
        Visible(bool isVisible) : isVisible(isVisible) {};

        std::vector<uint8_t> serialize() override {
            return { static_cast<uint8_t>(isVisible) };
        };

        void deserialize(std::vector<uint8_t> &data) override {
            if (data.size() != 1)
                throw std::runtime_error("Invalid data size for Visible component");

            isVisible = static_cast<bool>(data[0]);
        };

        size_t getSize() const override {
            return (1);
        };

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
