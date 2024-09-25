/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Sprite.cpp file
*/

#include "Sprite.hpp"

    std::string getTextureFile(uint32_t id) const {
        static const std::unordered_map<uint32_t, std::string> textureMap = {
            {0, "assets/sprite0.png"},
            {1, "assets/sprite1.png"},
            {2, "assets/sprite2.png"},
            {3, "assets/sprite3.png"},
            {9999, "assets/default.png"}
        };

        auto it = textureMap.find(id);
        if (it != textureMap.end()) {
            return it->second;
        } else {
            return "assets/default.png";
        }
    }

extern "C" IComponent *entryPoint(uint32_t texture_id)
{
    return new Sprite(texture_id);
}
