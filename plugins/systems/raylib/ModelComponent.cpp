/*
** EPITECH PROJECT, 2024
** RType
** File description:
** ModelComponent.cpp file
*/

#include "ModelComponent.hpp"
#include <iostream>

namespace Components {
    void ModelComponent::setModel(std::string &modelPath)
    {
        try {
            model = LoadModel(modelPath.c_str());
            isLoaded = true;
        } catch (std::runtime_error &e) {
            std::cerr << "Error while loading model: " << modelPath << std::endl;
            throw e;
        }
    }
};
