/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ISystem.hpp file
*/

#ifndef ISYSTEM_HPP
    #define ISYSTEM_HPP

namespace ECS {
    class Registry;
};

namespace Systems {
    class ISystem {
        public:
            virtual ~ISystem() = default;

            /**
             * @brief Function representing the system that will be called by the ECS
             * 
             * @param reg : The registry of the ECS
             * @note This function should contain the logic of the system
             */
            virtual void run(ECS::Registry &) = 0;
        private:
    };
};

#endif // ISYSTEM_HPP
