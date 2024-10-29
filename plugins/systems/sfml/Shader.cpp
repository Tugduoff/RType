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
        throw("Missing name attribute on shader setting");
    if (!shaderSetting.lookupValue("path", path))
        throw("Missing path attribute on shader setting");

    if (!shader.loadFromFile(path, sf::Shader::Fragment))
        throw std::runtime_error("Failed to load shader " + name + " from " + path);
    std::cerr << "Loaded shader " << name << " from " << path << std::endl;
}
