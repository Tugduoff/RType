/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ASystem.hpp file
*/

#ifndef ASYSTEM_HPP
    #define ASYSTEM_HPP

    #include "ISystem.hpp"

namespace Systems {
    /**
     * @class ASystem
     * 
     * @brief Abstract class for all our Systems.
     * 
     * This class is an abstract class for all ECS systems.
     * It does not provide any additional methods yet.
     */
    class ASystem : public ISystem {
        public:
            ASystem() = default;
        private:
    };
};

#endif // ASYSTEM_HPP
