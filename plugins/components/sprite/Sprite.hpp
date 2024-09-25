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
    #include <unordered_map>

/**
 * @brief Sprite component class for ECS.
 * 
 * This class handles the loading, serialization, and deserialization of textures and sprites
 * using SFML, for use in our ECS.
 */
class Sprite : public AComponent {
public:
    /**
     * @brief Constructs a new Sprite object.
     * 
     * Loads the texture from the file based on the provided texture ID and sets it to the sprite.
     * 
     * @param textureID The ID of the texture to be used by the sprite.
     * @throws std::runtime_error If the texture cannot be loaded from the specified file.
     */
    Sprite(uint32_t textureID) : textureID(textureID) {
        if (!texture.loadFromFile(getTextureFile(textureID))) {
            throw std::runtime_error("Failed to load texture");
        }
        sprite.setTexture(texture);
    }

    /**
     * @brief Default destructor for the Sprite component.
     */
    ~Sprite() = default;

    /**
     * @brief Serializes the Sprite's texture ID into a byte vector.
     * 
     * Converts the texture ID into network byte order for transmission or storage.
     * 
     * @return A vector of bytes representing the serialized texture ID.
     */
    std::vector<uint8_t> serialize() override {
        __network.textureID = htonl(textureID);
        return std::vector<uint8_t>(__data, __data + sizeof(__data));
    }

    /**
     * @brief Deserializes the texture ID from the provided byte vector.
     * 
     * Reads the texture ID in network byte order and loads the appropriate texture.
     * 
     * @param data A vector of bytes representing the serialized texture ID.
     * @throws std::runtime_error If the data size is invalid or the texture cannot be loaded.
     */
    void deserialize(std::vector<uint8_t> &data) override {
        if (data.size() != sizeof(__data))
            throw std::runtime_error("Invalid data size for sprite component");
        textureID = ntohl(*reinterpret_cast<uint32_t *>(data.data()));
        if (!texture.loadFromFile(getTextureFile(textureID))) {
            throw std::runtime_error("Failed to load texture");
        }
        sprite.setTexture(texture);
    }

    /**
     * @brief Gets the size of the serialized data.
     * 
     * @return The size of the data, in bytes.
     */
    size_t getSize() const override {
        return sizeof(__data);
    }

    /**
     * @brief Gets the SFML sprite object.
     * 
     * @return A constant reference to the SFML sprite.
     */
    const sf::Sprite& getSprite() const {
        return sprite;
    }

    /**
     * @brief Sets the position of the sprite.
     * 
     * @param x The X coordinate for the sprite's position.
     * @param y The Y coordinate for the sprite's position.
     */
    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }

    /**
     * @brief Gets the texture ID.
     * 
     * @return The texture ID currently assigned to the sprite.
     */
    uint32_t getTextureID() const {
        return textureID;
        }

    sf::Texture texture;
    sf::Sprite sprite;
    uint32_t textureID;
private:
    /**
     * @brief Returns the file path for a given texture ID.
     * 
     * This method maps a texture ID to a corresponding texture file.
     * 
     * @param id The texture ID.
     * @return The file path of the texture as a string.
     */
    std::string getTextureFile(uint32_t id) const;


    union {
        struct {
            uint32_t textureID;
        } __network;
        uint8_t __data[4];
    };
};

#endif // SPRITE_HPP
