/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Shader.cpp file
*/

#include "Shader.hpp"
#include <iostream>

Shader::Shader(const libconfig::Setting &shaderSetting)
{
    std::string path;

    if (!shaderSetting.lookupValue("name", name))
        std::cerr << "Missing name attribute on shader setting" << std::endl;
    if (!shaderSetting.lookupValue("path", path))
        std::cerr << "Missing path attribute on shader setting" << std::endl;

    if (!shader.loadFromFile(path, sf::Shader::Fragment))
        std::cerr << "Failed to load shader " << name << " from " << path << std::endl;
    std::cout << "Loaded shader " << name << " from " << path << std::endl;
}
