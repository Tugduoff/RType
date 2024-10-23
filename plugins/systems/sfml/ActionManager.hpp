/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ActionManager.hpp file
*/

#ifndef ACTION_MANAGER_HPP
    #define ACTION_MANAGER_HPP

    #include <SFML/Graphics.hpp>
    #include <libconfig.h++>
    #include "GameEngine/GameEngine.hpp"
    #include <unordered_map>
    #include "Texture.hpp"

namespace Systems {
    /**
     * @class ActionManager
     * 
     * @brief System class from Systems that manages entity actions.
     * 
     * This system is responsible for managing entity actions and setting the correct textures.
     */
    class ActionManager {
        public:

            ActionManager();
            ActionManager(libconfig::Setting &config);
            ~ActionManager() = default;

            void run(Engine::GameEngine &engine);
            void init(Engine::GameEngine &engine);

        private:

    };
};

#endif // ACTION_MANAGER_HPP
