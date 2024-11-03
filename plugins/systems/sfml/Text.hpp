/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Text.hpp file
*/

#ifndef TEXT_HPP
    #define TEXT_HPP

    #include "plugins/components/AComponent.hpp"
    #include <SFML/Graphics/Text.hpp>
    #include <SFML/Graphics/Font.hpp>
    #include <iostream>
    #include <string>
    #include <vector>

namespace Components {

    class Text {
        public:
            Text(const std::string &text = "Hello World", unsigned int size = 30, const sf::Color &color = sf::Color::White)
                : textContent(text), textSize(size), textColor(color)
            {
                #ifdef _WIN32
                    const std::string fontPath = "assets\\font.ttf";
                #else
                    const std::string fontPath = "assets/font.ttf";
                #endif

                if (!font.loadFromFile(fontPath))
                    std::cerr << "Error loading font from path: " << fontPath << std::endl;

                sfText.setFont(font);
                sfText.setString(textContent);
                sfText.setCharacterSize(textSize);
                sfText.setFillColor(textColor);
            }

            void setText(const std::string &text) {
                textContent = text;
                sfText.setString(textContent);
            }

            void setSize(unsigned int size) {
                textSize = size;
                sfText.setCharacterSize(textSize);
            }

            void setColor(const sf::Color &color) {
                textColor = color;
                sfText.setFillColor(textColor);
            }

            void setPosition(float x, float y) {
                sfText.setPosition(x, y);
            }

            sf::Text sfText;
            std::string textContent;
            unsigned int textSize;
            sf::Color textColor;
            sf::Font font;
    };
}

#endif // TEXT_HPP
