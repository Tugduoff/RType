/*
** EPITECH PROJECT, 2024
** RType
** File description:
** IClientComponent.hpp file
*/

#ifndef ICLIENT_COMPONENT_HPP
    #define ICLIENT_COMPONENT_HPP

    #include "IComponent.hpp"
    #include "Engine/ClientEngine.hpp"
    #include <stdexcept>

namespace Components {
    class IClientComponent : public IComponent {
        public:
            virtual ~IClientComponent() = default;

            void addTo(ECS::Entity &to, Engine::IEngine &engine, std::vector<std::any> args) override {
                Engine::ClientEngine* clientEngine = dynamic_cast<Engine::ClientEngine*>(&engine);
                if (clientEngine) {
                    addTo(to, *clientEngine, args);
                } else {
                    throw std::runtime_error("Invalid engine type, expected ClientEngine");
                }
            }

            void addTo(ECS::Entity &to, Engine::IEngine &engine, libconfig::Setting &config) override {
                Engine::ClientEngine* clientEngine = dynamic_cast<Engine::ClientEngine*>(&engine);
                if (clientEngine) {
                    addTo(to, *clientEngine, config);
                } else {
                    throw std::runtime_error("Invalid engine type, expected ClientEngine");
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
            virtual void addTo(ECS::Entity &to, Engine::ClientEngine &engine, std::vector<std::any> args) = 0;

            /**
             * @brief Add the component to an entity
             * 
             * @param to : The entity to add the component to
             * @param engine : The game engine
             * @param args : The arguments to pass to the component constructor
             * 
             * @note This function will add the component to the entity
             */
            virtual void addTo(ECS::Entity &to, Engine::ClientEngine &engine, libconfig::Setting &config) = 0;
    };
};

#endif // ICLIENT_COMPONENT_HPP
