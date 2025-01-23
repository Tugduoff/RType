/*
** EPITECH PROJECT, 2024
** RType
** File description:
** SpriteComponent.hpp file
*/

#ifndef SPRITECOMPONENT_HPP
    #define SPRITECOMPONENT_HPP

    #include "RayTexture.hpp"
    #include "raylib.h"

namespace Components {
    class SpriteComponent {
    public:
        SpriteComponent() : nextTexture("IDLE"), currentAction("NONE") {};

        void addTexture(RayTexture &texture) {
            textures.push_back(texture);
            rect = texture.getRect();

            if (currentAction == "NONE") {
                currentTexture = texture;
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
                // std::cerr << "Current texture is done for action: " << currentTexture.getAction() << std::endl;

                // Find the texture corresponding to the new action
                bool textureFound = false;
                for (auto &texture : textures) {
                    if (texture.getAction() == action) {
                        currentTexture = texture;  // Switch to the new texture
                        currentAction = action;
                        currentTexture.restartChrono();
                        // std::cerr << "Loaded texture for action: " << action << std::endl;
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
        };

        std::vector<RayTexture> textures;
        RayTexture currentTexture;
        std::string nextTexture;
        std::string currentAction;
        Vector2 position;
        Rectangle rect;
    
    };
};

#endif // SPRITECOMPONENT_HPP
