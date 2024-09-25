/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Sprite.hpp file
*/

#ifndef SPRITE_HPP
    #define SPRITE_HPP

    #include "AComponent.hpp"
    #include <SFML/Graphics.hpp>
    #include <stdexcept>
    #include <arpa/inet.h>
    #include <vector>

class Sprite : public AComponent {
public:
    Sprite(uint32_t textureID) : textureID(textureID) {
        if (!texture.loadFromFile(getTextureFile(textureID))) {
            throw std::runtime_error("Failed to load texture");
        }
        sprite.setTexture(texture);
    }
    ~Sprite() = default;

    std::vector<uint8_t> serialize() override {
        __network.textureID = htonl(textureID);
        return std::vector<uint8_t>(__data, __data + sizeof(__data));
    }

    void deserialize(std::vector<uint8_t> &data) override {
        if (data.size() != sizeof(__data))
            throw std::runtime_error("Invalid data size for sprite component");
        textureID = ntohl(*reinterpret_cast<uint32_t *>(data.data()));
        if (!texture.loadFromFile(getTextureFile(textureID))) {
            throw std::runtime_error("Failed to load texture");
        }
        sprite.setTexture(texture);
    }

    size_t getSize() const override { return sizeof(__data); }

    const sf::Sprite& getSprite() const {
        return sprite;
    }
    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }
    uint32_t getTextureID() const {
        return textureID;
    }

    sf::Texture texture;
    sf::Sprite sprite;
    uint32_t textureID;
private:

    std::string getTextureFile(uint32_t id) const {
        switch (id) {
            case 0:
                return "assets/sprite0.png";
            case 1:
                return "assets/sprite1.png";
            default:
                return "assets/default.png";
        }
    }
    union {
        struct {
            uint32_t textureID;
        } __network;
        uint8_t __data[4];
    };
};

#endif // SPRITE_HPP
