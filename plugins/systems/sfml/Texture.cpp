/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Texture.cpp file
*/

#include "Texture.hpp"

Texture::Texture(
    unsigned updateRate,
    unsigned width,
    unsigned height,
    const std::string &texturePath,
    const std::string &action,
    bool repeat,
    bool resetBeforeEnd) :
    __chrono(Chrono()),
    __updateRate(updateRate),
    __rect(sf::IntRect(0, 0, width, height)),
    __action(action),
    __repeat(repeat),
    __actionDone(false),
    __resetBeforeEnd(resetBeforeEnd),
    __isEnd(false),
    __fullyDone(false)
{
    if (!__texture.loadFromFile(texturePath)) {
        std::cerr << "Error: Could not load texture from path: " << texturePath << std::endl;
    }
}

void Texture::update(sf::Sprite &sprite) {
    std::cerr << "TEXTURE UPDATE: " << __updateRate << " " << __rect.left << " " << __action << " " << __actionDone << std::endl;

    if (__fullyDone) {
        return;
    }
    if (sprite.getTexture() != &__texture)
        sprite.setTexture(__texture);  // Ensure correct texture is set

    if (__isEnd && __resetBeforeEnd) {
        if (__chrono.getElapsedTime() > (size_t)__updateRate) {
            __chrono.restart();
            if (__rect.left > 0) {
                __rect.left -= __rect.width;
            } else {
                __isEnd = false;
                __actionDone = false;
                __rect.left = 0;
                __fullyDone = true;
                std::cerr << "Animation fully done" << std::endl;
            }
        }
    } else if (__isEnd) {
        __fullyDone = true;
    }

    // Update the animation if it's not done yet
    if (!__isEnd && (__updateRate != 0 || __repeat || (!__repeat && !__actionDone))) {
        if (__chrono.getElapsedTime() > (size_t)__updateRate) {
            __chrono.restart();
            __rect.left += __rect.width;
            if ((unsigned)__rect.left >= __texture.getSize().x) {
                if (__repeat) {
                    __rect.left = 0;  // Loop back to the beginning
                } else {
                    __rect.left -= __rect.width;  // Stop at the last frame
                }
                __actionDone = true;
                std::cerr << "Action " << __action << " done" << std::endl;
            }
        }
    }

    // Update sprite's texture rectangle
    sprite.setTextureRect(__rect);
    sprite.setOrigin(__rect.width / 2, __rect.height / 2);
}
