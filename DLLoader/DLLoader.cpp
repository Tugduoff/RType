/*
** EPITECH PROJECT, 2024
** B-CPP-500-NAN-5-1-rtype-thomas.cluseau
** File description:
** DLLoader
*/

#include <iostream>
#include <memory>
#include "DLLoader.hpp"
#ifdef _WIN32
    #include <stringapiset.h>
#endif

DLLoader::DLLoader(const std::string &libName) {
    #ifdef _WIN32
        __library = LoadLibraryA(libName.c_str());
        if (!__library) throw DLLExceptions("Failed to load library: " + libName);
    #else
        __library = dlopen(libName.c_str(), RTLD_LAZY);
        if (!__library) throw DLLExceptions(dlerror());
    #endif
}

DLLoader::~DLLoader() {
    if (__library) {
        #ifdef _WIN32
            if (!FreeLibrary((HMODULE)__library)) {
                std::cerr << "Error while closing the library." << std::endl;
                exit(84);
            }
        #else
            if (dlclose(__library) != 0) {
                std::cerr << dlerror() << std::endl;
                exit(84);
            }
        #endif
    }
}

void DLLoader::loadNew(const std::string &libName) {
    if (__library) {
        #ifdef _WIN32
            if (!FreeLibrary((HMODULE)__library)) throw DLLExceptions("Error while closing the library.");
        #else
            if (dlclose(__library) != 0) throw DLLExceptions(dlerror());
        #endif
    }

    #ifdef _WIN32
        __library = LoadLibraryA(libName.c_str()); // Use LoadLibraryA for ANSI strings
        if (!__library) throw DLLExceptions("Failed to load library: " + libName);
    #else
        __library = dlopen(libName.c_str(), RTLD_LAZY);
        if (!__library) throw DLLExceptions(dlerror());
    #endif
}
