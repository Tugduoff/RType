/*
** EPITECH PROJECT, 2024
** RType
** File description:
** AComponent.hpp file
*/

#ifndef ACOMPONENT_HPP
    #define ACOMPONENT_HPP

    #include "IComponent.hpp"

    #ifdef _WIN32
        #define WINDOWS_DLL_EXPORT __declspec(dllexport)
    #else
        #define WINDOWS_DLL_EXPORT
    #endif

namespace Components {
    class AComponent : public IComponent {
        public:
            AComponent() = default;
        private:
    };
}

#endif // ACOMPONENT_HPP
