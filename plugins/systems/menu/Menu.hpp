/*
** EPITECH PROJECT, 2024
** RType
** File description:
** MenuSystem.hpp file
*/

#ifndef MENU_HPP
    #define MENU_HPP

    #include <libconfig.h++>
    #include "systems/ASystem.hpp"

namespace Engine {
    class GameEngine;
}

namespace Systems {

    class MenuSystem : public ASystem {
        public:
            MenuSystem() = default;
            MenuSystem(libconfig::Setting &config);
            ~MenuSystem() = default;

            void run(Engine::GameEngine &engine) override;
            void init(Engine::GameEngine &engine) override;
        
        private:
            bool _titleReachedLeftEdge;
    };
};

#endif // MENU_HPP_
