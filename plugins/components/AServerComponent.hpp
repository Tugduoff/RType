/*
** EPITECH PROJECT, 2024
** RType
** File description:
** AServerComponent.hpp file
*/

#ifndef ASERVER_COMPONENT_HPP
    #define ASERVER_COMPONENT_HPP

    #include "IServerComponent.hpp"

namespace Components {
    class AServerComponent : public IServerComponent {
        public:

            AServerComponent() = default;
            ~AServerComponent() = default;

        private:
    };
};

#endif // ASERVER_COMPONENT_HPP
