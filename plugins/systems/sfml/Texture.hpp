/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Texture.hpp file
*/

#ifndef TEXTURE_HPP
    #define TEXTURE_HPP

    #include <SFML/Graphics.hpp>
    #include <SFML/Window.hpp>
    #include <SFML/System.hpp>
    #include <libconfig.h++>
    #include "GameEngine/GameEngine.hpp"
    #include "utils/Chrono.hpp"

/**
 * @class Texture
 * 
 * @brief Texture class that handles textures.
 * 
 * This class is responsible for handling textures and updating them.
 * It can be used to load textures from files and update them.
 */
class Texture {
    public:

        /**
         * @brief Construct a new Texture object
         * 
         * @param updateRate : the rate at which the texture should be updated
         * @param width : the width of the texture
         * @param height : the height of the texture
         * @param texturePath : the path to the texture file
         * @param action : the action of the texture defining its id
         * @param repeat : whether the texture animation should repeat
         * @param resetBeforeEnd : whether the texture should reset before the end
         */
        Texture(
            unsigned updateRate = 100,
            unsigned width = 0,
            unsigned height = 0,
            const std::string &texturePath = "",
            const std::string &action = "UNKNOWN",
            bool repeat = false,
            bool resetBeforeEnd = false);
        ~Texture() = default;

        /**
         * @brief Get the texture
         * 
         * @return sf::Texture& The texture
         */
        sf::Texture &getTexture() { return __texture; };

        /**
         * @brief Update the texture
         * 
         * @param sprite : the sprite to update
         * 
         * @note This function will update the texture of the sprite.
         * @note It will update the texture rect of the sprite.
         * @note It will update the texture based on: the update rate.
         * @note the action.
         * @note the repeat flag.
         * @note the reset before end flag.
         */
        void update(sf::Sprite &sprite);

        /**
         * @brief Check if the texture is loaded
         * 
         * @return true : the texture is loaded
         * 
         * @note This function will return true if the texture is loaded.
         */
        bool isLoaded() { return __texture.getSize().x != 0; };

        /**
         * @brief Get the action of the texture
         * 
         * @return std::string& The action of the texture
         * 
         * @note This function will return the action of the texture.
         */
        std::string &getAction() { return __action; };

        /**
         * @brief Restart the chrono
         * 
         * @note This function will restart the chrono of the animation.
         */
        void restartChrono() { __chrono.restart(); };

        /**
         * @brief Initialize the end of the texture
         * 
         * @note This function will initialize the end of the texture.
         */
        void initEnd() { __isEnd = true; };

        /**
         * @brief Check if the texture's animation is fully done
         * 
         * @return true : the texture's animation is fully done
         * 
         * @note This function will return true if the texture's animation is fully complete.
         */
        bool isDone() { return __fullyDone; };

        void stop() { __fullyDone = true; };

    private:

        sf::Texture __texture;
        Chrono __chrono;
        unsigned __updateRate;
        sf::IntRect __rect;
        std::string __action;
        bool __repeat;
        bool __actionDone;
        bool __resetBeforeEnd;
        bool __isEnd;
        bool __fullyDone;
};

#endif // TEXTURE_HPP
