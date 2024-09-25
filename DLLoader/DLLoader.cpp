/*
** EPITECH PROJECT, 2024
** B-CPP-500-NAN-5-1-rtype-thomas.cluseau
** File description:
** DLLoader
*/

#include "DLLoader.hpp"

DLLoader(const std::string &libName) : __library(dlopen(libName.c_str(), RTLD_LAZY)) {
    if (!__library) throw DLLExceptions(dlerror());
}

~DLLoader() {
    if (__library)
        if (dlclose(__library) != 0) std::cerr << dlerror() << std::endl;
}

void loadNew(const std::string &libName) {
    if (__library)
        if (dlclose(__library) != 0) throw DLLExceptions(dlerror());
    __library = dlopen(libName.c_str(), RTLD_LAZY);
    if (!__library) throw DLLExceptions(dlerror());
}
