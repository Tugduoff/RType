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
    /**
     * @class MenuSystem
     * 
     * @brief System class from Systems that Menus entities.
     * 
     * This system is responsible for moving entities based on their velocity and position components.
     */
    class MenuSystem : public ASystem {
        public:
            MenuSystem() = default;
            MenuSystem(libconfig::Setting &config);
            ~MenuSystem() = default;

            void run(Engine::GameEngine &engine) override;
            void init(Engine::GameEngine &engine) override;
        private:
    };
};

#endif // MENU_HPP_
