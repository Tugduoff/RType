/*
** EPITECH PROJECT, 2024
** RType
** File description:
** RayTexture.cpp file
*/

#include "RayTexture.hpp"

RayTexture::RayTexture(
    unsigned updateRate,
    unsigned width,
    unsigned height,
    const std::string &texturePath,
    const std::string &action,
    bool repeat,
    bool resetBeforeEnd) :
    __chrono(Chrono()),
    __updateRate(updateRate),
    __rect({0, 0, (float)width, (float)height}),
    __action(action),
    __repeat(repeat),
    __actionDone(false),
    __resetBeforeEnd(resetBeforeEnd),
    __isEnd(false),
    __fullyDone(false)
{
    __texture = LoadTexture(texturePath.c_str());
}

void RayTexture::update(Texture2D &texture)
{
    if (__fullyDone) {
        return;
    }
    if (texture.id != __texture.id)
        texture = __texture;  // Ensure correct texture is set

    if (__isEnd && __resetBeforeEnd) {
        if (__chrono.getElapsedTime() > (size_t)__updateRate) {
            __chrono.restart();
            if (__rect.x > 0) {
                __rect.x -= __rect.width;
            } else {
                __isEnd = false;
                __actionDone = false;
                __rect.x = 0;
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
            if (__rect.x < __texture.width - __rect.width) {
                __rect.x += __rect.width;
            } else {
                if (__repeat) {
                    __rect.x = 0;
                } else {
                    __isEnd = true;
                    __actionDone = true;
                }
            }
        }
    }
}
