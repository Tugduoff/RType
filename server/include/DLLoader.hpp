/*
** EPITECH PROJECT, 2024
** B-CPP-500-NAN-5-1-rtype-thomas.cluseau
** File description:
** DLLoader
*/

#ifndef DLLOADER_HPP
    #define DLLOADER_HPP

    #include <memory>
    #include <dlfcn.h>
    #include <iostream>

class DLLoader {
    public:
        DLLoader(const std::string &libName) : __library(dlopen(libName.c_str(), RTLD_LAZY)) {
            if (!__library) throw DLLExceptions(dlerror());
        };

        ~DLLoader() {
            if (__library) {
                if (dlclose(__library) != 0) {
                    std::cerr << dlerror() << std::endl;
                    exit(84);
                }
            }
        };

        void loadNew(const std::string &libName) {
            if (__library) {
                if (dlclose(__library) != 0) throw DLLExceptions(dlerror());
            }
            __library = dlopen(libName.c_str(), RTLD_LAZY);
            if (!__library) throw DLLExceptions(dlerror());
        };

        template<typename T, typename... Args>
        T *getInstance(const std::string &entryPointName = "entryPoint", Args&&... args) {
            using EntryPointFunc = T* (*)(Args...);
            EntryPointFunc entryPoint = reinterpret_cast<EntryPointFunc>(dlsym(__library, entryPointName.c_str()));

            if (!entryPoint) throw DLLExceptions(dlerror());

            return entryPoint(std::forward<Args>(args)...);
        };

        class DLLExceptions : public std::exception {
            public:
                DLLExceptions(const std::string &error) : _errMsg(error) {}

                const char *what() const noexcept override { return _errMsg.c_str(); }

            private:
                std::string _errMsg = "Unexpected error occurred";
        };

    private:
        void *__library;
};

#endif /* !DLLOADER_HPP_ */
