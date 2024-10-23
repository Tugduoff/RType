/*
** EPITECH PROJECT, 2024
** RType
** File description:
** IComponent.hpp file
*/

#ifndef ICOMPONENT_HPP
    #define ICOMPONENT_HPP

    #include "ECS/entity/Entity.hpp"
    #include "ECS/utilities/SparseArray.hpp"
    #include <cstddef>
    #include <cstdint>
    #include <vector>
    #include <any>
    #include <libconfig.h++>

namespace Engine {
    class GameEngine;
}

/**
 * @namespace Components
 * @brief Namespace for all ECS Components.
 */
namespace Components {
    /**
     * @class IComponent
     * 
     * @brief Interface for all our Components.
     * 
     * This class is an interface for all ECS components. It provides methods to serialize and deserialize
     * components, add them to entities, and get their size.
     */
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
            virtual void addTo(ECS::Entity &to, Engine::GameEngine &engine, std::vector<std::any> args) = 0;

            /**
             * @brief Add the component to an entity
             * 
             * @param to : The entity to add the component to
             * @param engine : The game engine
             * @param args : The arguments to pass to the component constructor
             * 
             * @note This function will add the component to the entity
             */
            virtual void addTo(ECS::Entity &to, Engine::GameEngine &engine, libconfig::Setting &config) = 0;

            /**
             * @brief Cast a SparseArray out of a std::any
             *
             * This function is required for technical reasons: it allows the
             * client to call methods on `IComponent`s stored in the Registry.
             *
             * For the any_cast to be successful, the concrete implementer
             * must call std::any_cast<SparseArray<_concrete_>>. It should
             * then cast by not quite standard means the SparseArray<_concrete_>
             * into a SparseArray<IComponent>, which should be safe as long as
             * IComponent is the **first base class** of the concrete type.
             *
             * This is explained quite well by https://stackoverflow.com/a/38539705
             */
            virtual SparseArray<IComponent> &any_cast(std::any &sparseArray) = 0;
            virtual const SparseArray<IComponent> &any_cast(const std::any &sparseArray) = 0;

            virtual const std::string &getId() = 0;

        private:
    };
};

#endif // ICOMPONENT_HPP
