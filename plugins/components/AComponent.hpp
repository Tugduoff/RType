/*
** EPITECH PROJECT, 2024
** RType
** File description:
** AComponent.hpp file
*/

#ifndef ACOMPONENT_HPP
    #define ACOMPONENT_HPP

    #include "IComponent.hpp"

template <size_t Size>
class AComponent : public IComponent<Size> {
    public:
        AComponent() = default;
    private:
};

#endif // ACOMPONENT_HPP
