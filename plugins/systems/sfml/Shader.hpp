/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Shader.hpp file
*/

#ifndef SHADER_HPP
    #define SHADER_HPP

    #include <SFML/Graphics.hpp>
    #include <libconfig.h++>

class Shader {
    public:

        Shader(const libconfig::Setting &shaderSetting);
        ~Shader() = default;

        std::string name;
        sf::Shader shader;

};

#endif // SHADER_HPP
