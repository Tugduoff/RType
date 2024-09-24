/*
** EPITECH PROJECT, 2024
** RType
** File description:
** IComponent.hpp file
*/

#ifndef ICOMPONENT_HPP
    #define ICOMPONENT_HPP

    #include <cstddef>
    #include <span>
    #include <cstdint>

template <size_t Size>
class IComponent {
    public:
        virtual ~IComponent() = default;

        /**
         * @brief Serialize the component
         * 
         * @return std::span<uint8_t, Size> : The serialized data
         * @note The size of the span should be equal to the size of the component (getSize())
         */
        virtual std::span<uint8_t, Size> serialize() = 0;

        /**
         * @brief Deserialize the component
         * 
         * @param data : The data to deserialize
         * @note The size of the span should be equal to the size of the component (getSize())
         */
        virtual void deserialize(std::span<uint8_t, Size> &data) = 0;

        /**
         * @brief Get the size of the component
         * 
         * @return size_t : The size of the component
         */
        static constexpr size_t getSize() { return Size; };
    private:
};

#endif // ICOMPONENT_HPP
