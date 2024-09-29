/*
** EPITECH PROJECT, 2024
** RType
** File description:
** DLLoader
*/

#include <iostream>
#include "DLLoader.hpp"

DLLoader::DLLoader(const std::string &libName) : __library(dlopen(libName.c_str(), RTLD_LAZY))
{
    if (!__library)
        throw DLLExceptions(dlerror());
}

DLLoader::DLLoader(DLLoader &&loader) : __library(loader.__library)
{
    loader.__library = nullptr;
}

DLLoader::~DLLoader()
{
    if (!__library)
        return;
    if (dlclose(__library) != 0)
        std::cerr << dlerror() << std::endl;
}

void DLLoader::loadNew(const std::string &libName)
{
    if (!__library) {
        __library = dlopen(libName.c_str(), RTLD_LAZY);
        if (!__library)
            throw DLLExceptions(dlerror());
        return;
    }
    if (dlclose(__library) != 0)
        throw DLLExceptions(dlerror());
    __library = dlopen(libName.c_str(), RTLD_LAZY);
    if (!__library)
        throw DLLExceptions(dlerror());
}
