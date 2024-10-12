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
    /**
     * @class AComponent
     * 
     * @brief Abstract class for all our Components.
     * 
     * This class is an abstract class for all ECS components. It provides a method to get the component's ID.
     * It stores the component's name on ctor as a stringID.
     */
    class AComponent : public IComponent {
        public:
            AComponent(std::string componentName) : __componentName(componentName) {};

            const std::string &getId() { return __componentName; };

        private:

            const std::string __componentName;
    };
}

#endif // ACOMPONENT_HPP
