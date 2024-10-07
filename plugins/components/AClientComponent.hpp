/*
** EPITECH PROJECT, 2024
** RType
** File description:
** AClientComponent.hpp file
*/

#ifndef ACLIENT_COMPONENT_HPP
    #define ACLIENT_COMPONENT_HPP

    #include "IClientComponent.hpp"

namespace Components {
    class AClientComponent : public IClientComponent {
        public:

            AClientComponent() = default;
            ~AClientComponent() = default;

        private:
    };
};

#endif // ACLIENT_COMPONENT_HPP
