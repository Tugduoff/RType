/*
** EPITECH PROJECT, 2024
** RType
** File description:
** SpriteComponent.hpp file
*/

#ifndef SPRITECOMPONENT_HPP
    #define SPRITECOMPONENT_HPP

    #include <SFML/Graphics/Sprite.hpp>
    #include "Texture.hpp"

namespace Components {
    class SpriteComponent {
    public:
        SpriteComponent() : sprite(), nextTexture("IDLE"), currentAction("NONE") {};

        void addTexture(Texture &texture) {
            textures.push_back(texture);

            if (currentAction == "NONE") {
                currentTexture = texture;
                sprite.setTexture(currentTexture.getTexture());
                currentTexture.restartChrono();
                currentAction = currentTexture.getAction();
            }
        }

        void loadTextureForAction(const std::string &action) {
            if (currentTexture.getAction() == action) {
                nextTexture = "";  // If the current texture is the same, do nothing
                return;
            }

            if (currentAction == "NONE") {
                std::cerr << "No texture loaded yet" << std::endl;
                return;
            }

            // Check if the current texture is done before switching
            if (currentTexture.isDone()) {
                std::cerr << "Current texture is done for action: " << currentTexture.getAction() << std::endl;

                // Find the texture corresponding to the new action
                bool textureFound = false;
                for (auto &texture : textures) {
                    if (texture.getAction() == action) {
                        currentTexture = texture;  // Switch to the new texture
                        currentAction = action;
                        sprite.setTexture(currentTexture.getTexture());
                        currentTexture.restartChrono();
                        std::cerr << "Loaded texture for action: " << action << std::endl;
                        nextTexture = "";  // Clear the next texture
                        textureFound = true;
                        break;
                    }
                }
            } else {
                currentTexture.initEnd();
                nextTexture = action;  // Set the next texture to be loaded later
            }
        }

        void stopCurrentAction() {
            if (currentAction != "NONE")
                currentTexture.stop();
        }

        void update() {
            // Load the next texture if needed
            if (!nextTexture.empty()) {
                loadTextureForAction(nextTexture);
            }

            // Update the current texture's animation
            currentTexture.update(sprite);
        };

        sf::Sprite sprite;
        std::vector<Texture> textures;
        Texture currentTexture;
        std::string nextTexture;
        std::string currentAction;
    
    };
};

#endif // SPRITECOMPONENT_HPP
