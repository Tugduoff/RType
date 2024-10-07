/*
** EPITECH PROJECT, 2024
** RType
** File description:
** IComponent.hpp file
*/

#ifndef ICOMPONENT_HPP
    #define ICOMPONENT_HPP

    #include "ECS/entity/Entity.hpp"
    #include <cstddef>
    #include <cstdint>
    #include <vector>
    #include <any>
    #include <libconfig.h++>

namespace Engine {
    class IEngine;
}

namespace Components {
    class IComponent {
        public:
            virtual ~IComponent() = default;

            /**
             * @brief Serialize the component
             * 
             * @return std::span<uint8_t, Size> : The serialized data
             * @note The size of the span should be equal to the size of the component (getSize())
             */
            virtual std::vector<uint8_t> serialize() = 0;

            /**
             * @brief Deserialize the component
             * 
             * @param data : The data to deserialize
             * @note The size of the span should be equal to the size of the component (getSize())
             * @throw std::runtime_error : If the size of the data is invalid
             */
            virtual void deserialize(std::vector<uint8_t> &data) = 0;

            /**
             * @brief Get the size of the component
             * 
             * @return size_t : The size of the component
             */
            virtual size_t getSize() const = 0;

            /**
             * @brief Add the component to an entity
             * 
             * @param to : The entity to add the component to
             * @param engine : The game engine
             * @param args : The arguments to pass to the component constructor
             * 
             * @note This function will add the component to the entity
             */
            virtual void addTo(ECS::Entity &to, Engine::IEngine &engine, std::vector<std::any> args) = 0;

            /**
             * @brief Add the component to an entity
             * 
             * @param to : The entity to add the component to
             * @param engine : The game engine
             * @param args : The arguments to pass to the component constructor
             * 
             * @note This function will add the component to the entity
             */
            virtual void addTo(ECS::Entity &to, Engine::IEngine &engine, libconfig::Setting &config) = 0;
        private:
    };
};

#endif // ICOMPONENT_HPP
