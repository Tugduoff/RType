/*
** EPITECH PROJECT, 2024
** RType
** File description:
** IServerComponent.hpp file
*/

#ifndef ISERVER_COMPONENT_HPP
    #define ISERVER_COMPONENT_HPP

    #include "IComponent.hpp"
    #include "Engine/ServerEngine.hpp"
    #include <stdexcept>

namespace Components {
    class IServerComponent : public IComponent {
        public:
            virtual ~IServerComponent() = default;

            void addTo(ECS::Entity &to, Engine::IEngine &engine, std::vector<std::any> args) override {
                Engine::ServerEngine* serverEngine = dynamic_cast<Engine::ServerEngine*>(&engine);
                if (serverEngine) {
                    addTo(to, *serverEngine, args);
                } else {
                    throw std::runtime_error("Invalid engine type, expected ServerEngine");
                }
            }

            void addTo(ECS::Entity &to, Engine::IEngine &engine, libconfig::Setting &config) override {
                Engine::ServerEngine* serverEngine = dynamic_cast<Engine::ServerEngine*>(&engine);
                if (serverEngine) {
                    addTo(to, *serverEngine, config);
                } else {
                    throw std::runtime_error("Invalid engine type, expected ServerEngine");
                }
            }

            /**
             * @brief Add the component to an entity
             * 
             * @param to : The entity to add the component to
             * @param engine : The game engine
             * @param args : The arguments to pass to the component constructor
             * 
             * @note This function will add the component to the entity
             */
            virtual void addTo(ECS::Entity &to, Engine::ServerEngine &engine, std::vector<std::any> args) = 0;

            /**
             * @brief Add the component to an entity
             * 
             * @param to : The entity to add the component to
             * @param engine : The game engine
             * @param args : The arguments to pass to the component constructor
             * 
             * @note This function will add the component to the entity
             */
            virtual void addTo(ECS::Entity &to, Engine::ServerEngine &engine, libconfig::Setting &config) = 0;
    };
};

#endif // ISERVER_COMPONENT_HPP
