/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ASystem.hpp file
*/

#ifndef ASYSTEM_HPP
    #define ASYSTEM_HPP

    #include "ISystem.hpp"

    #ifdef _WIN32
        #define WINDOWS_DLL_EXPORT __declspec(dllexport)
    #else
        #define WINDOWS_DLL_EXPORT
    #endif

namespace Systems {
    class ASystem : public ISystem {
        public:
            ASystem() = default;
        private:
    };
};

#endif // ASYSTEM_HPP
