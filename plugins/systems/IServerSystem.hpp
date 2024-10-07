/*
** EPITECH PROJECT, 2024
** RType
** File description:
** IServerSystem.hpp file
*/

#ifndef ISERVER_SYSTEM_HPP
    #define ISERVER_SYSTEM_HPP

    #include "ISystem.hpp"
    #include "Engine/ServerEngine.hpp"
    #include <stdexcept>

namespace Systems {
    class IServerSystem : public ISystem {
        public:
            virtual ~IServerSystem() = default;

            void run(Engine::IEngine &engine) override {
                Engine::ServerEngine* serverEngine = dynamic_cast<Engine::ServerEngine*>(&engine);
                if (serverEngine) {
                    run(*serverEngine);
                } else {
                    throw std::runtime_error("Invalid engine type, expected ServerEngine");
                }
            }

            void init(Engine::IEngine &engine) override {
                Engine::ServerEngine* serverEngine = dynamic_cast<Engine::ServerEngine*>(&engine);
                if (serverEngine) {
                    init(*serverEngine);
                } else {
                    throw std::runtime_error("Invalid engine type, expected ServerEngine");
                }
            }

            virtual void run(Engine::ServerEngine &serverEngine) = 0;
            virtual void init(Engine::ServerEngine &serverEngine) = 0;
    };
};

#endif // ISERVER_SYSTEM_HPP
