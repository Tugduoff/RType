/*
** EPITECH PROJECT, 2024
** RType
** File description:
** DLLoader
*/

#ifdef _WIN32
    #include <stringapiset.h>
#endif
#include <iostream>
#include "DLLoader.hpp"

// --- PUBLIC --- //

DLLoader::DLLoader(const std::string &libName) {
    __library = openLibrary(libName);
}

DLLoader::~DLLoader() {
    if (__library)
        closeLibrary();        
}

void DLLoader::loadNew(const std::string &libName) {
    if (__library)
        closeLibrary();

    __library = openLibrary(libName);
}

// --- PRIVATE --- //

void DLLoader::DLLoader openLibrary(const std::string &libName) {
    #ifdef _WIN32
        __library = LoadLibraryA(libName.c_str());
        if (!__library) throw DLLExceptions("Failed to load library: " + libName);
    #else
        __library = dlopen(libName.c_str(), RTLD_LAZY);
        if (!__library) throw DLLExceptions(dlerror());
    #endif
}

void DLLoader::DLLoader closeLibrary() {
    #ifdef _WIN32
        if (!FreeLibrary((HMODULE)__library))
            std::cerr << "Error while closing the library." << std::endl;
    #else
        if (dlclose(__library) != 0)
            std::cerr << dlerror() << std::endl;
    #endif
}
