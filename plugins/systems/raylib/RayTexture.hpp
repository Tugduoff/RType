/*
** EPITECH PROJECT, 2024
** RType
** File description:
** RayTexture.hpp file
*/

#ifndef RAY_TEXTURE_HPP
    #define RAY_TEXTURE_HPP

    #include <string>
    #include <vector>
    #include "raylib.h"
    #include "utils/Chrono.hpp"

class RayTexture {
    public:

        RayTexture(
            unsigned updateRate = 100,
            unsigned width = 0,
            unsigned height = 0,
            const std::string &texturePath = "",
            const std::string &action = "UNKNOWN",
            bool repeat = false,
            bool resetBeforeEnd = false);
        ~RayTexture() = default;

        Texture2D &getTexture() { return __texture; };

        Rectangle &getRect() { return __rect; };

        void update(Texture2D &texture);

        bool isLoaded() { return __texture.width != 0; };

        std::string &getAction() { return __action; };

        void restartChrono() { __chrono.restart(); };

        void initEnd() { __isEnd = true; };

        bool isDone() { return __fullyDone; };

        void stop() { __fullyDone = true; };

    private:

        Texture2D __texture;
        Chrono __chrono;
        unsigned __updateRate;
        Rectangle __rect;
        std::string __action;
        bool __repeat;
        bool __actionDone;
        bool __resetBeforeEnd;
        bool __isEnd;
        bool __fullyDone;
};

#endif // RAY_TEXTURE_HPP