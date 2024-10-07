/*
** EPITECH PROJECT, 2024
** RType
** File description:
** IClientSystem.hpp file
*/

#ifndef ICLIENT_SYSTEM_HPP
    #define ICLIENT_SYSTEM_HPP

    #include "ISystem.hpp"
    #include "Engine/ClientEngine.hpp"
    #include <stdexcept>

namespace Systems {
    class IClientSystem : public ISystem {
        public:
            virtual ~IClientSystem() = default;

            void run(Engine::IEngine &engine) override {
                Engine::ClientEngine* clientEngine = dynamic_cast<Engine::ClientEngine*>(&engine);
                if (clientEngine) {
                    run(*clientEngine);
                } else {
                    throw std::runtime_error("Invalid engine type, expected clientEngine");
                }
            }

            void init(Engine::IEngine &engine) override {
                Engine::ClientEngine* clientEngine = dynamic_cast<Engine::ClientEngine*>(&engine);
                if (clientEngine) {
                    init(*clientEngine);
                } else {
                    throw std::runtime_error("Invalid engine type, expected clientEngine");
                }
            }

            virtual void run(Engine::ClientEngine &clientEngine) = 0;
            virtual void init(Engine::ClientEngine &clientEngine) = 0;
    };
};

#endif // ICLIENT_SYSTEM_HPP
