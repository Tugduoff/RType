/*
** EPITECH PROJECT, 2024
** RType
** File description:
** AComponent.hpp file
*/

#ifndef ACOMPONENT_HPP
    #define ACOMPONENT_HPP

    #include "IComponent.hpp"

namespace Components {
    class AComponent : public IComponent {
        public:
            AComponent(std::string componentName) : __componentName(componentName) {};

            std::string &getId() { return __componentName; };

        private:

            std::string __componentName;
    };
}

#endif // ACOMPONENT_HPP
