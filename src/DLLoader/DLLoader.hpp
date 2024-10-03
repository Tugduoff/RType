/*
** EPITECH PROJECT, 2024
** RType
** File description:
** DLLoader
*/

#ifndef DLLOADER_HPP
    #define DLLOADER_HPP

    #ifdef _WIN32
        #include <windows.h>
    #else
        #include <dlfcn.h>
    #endif
    #include <string>
    #include <utility>
    #include <memory>
    #include <iostream>

class DLLoader {
    public:

        /**
        * @brief Constructs a DLLoader object and opens the specified library.
        *
        * @param libName The name of the shared library to open.
        *
        * @throw DLLExceptions If the library cannot be opened.
        */
        DLLoader(const std::string &libPath, const std::string &libName)
        {
            openLibrary(libPath, libName);
        }

        /**
         * @brief Move constructor.
         * 
         * @param loader The DLLoader object to move.
         * 
         * @note The moved object will have its __library pointer set to nullptr.
         */
        DLLoader(DLLoader &&loader) : __library(loader.__library)
        {
            loader.__library = nullptr;
        }

        /**
        * @brief Destroys the DLLoader object and closes the currently opened library.
        *
        * @throw std::exception If an error occurs during closing the library.
        */
        ~DLLoader() {
            if (__library)
                closeLibrary();
        };

        /**
        * @brief Closes the currently opened library and opens a new library.
        *
        * @param libName The name of the new shared library to open.
        *
        * @throw DLLExceptions If the new library cannot be opened.
        */
        void loadNew(const std::string &libPath, const std::string &libName) {
            if (__library)
                closeLibrary();
            openLibrary(libPath, libName);
        };

        /**
        * @brief Retrieves a function pointer from the library and calls it to create and return a new instance of type T.
        *
        * @tparam T The type of the object to create.
        * @tparam Args The types of the arguments to pass to the function.
        *
        * @param entryPointName The name of the function to retrieve from the library (default: "entryPoint").
        * @param args The arguments to pass to the function.
        *
        * @return A pointer to a new instance of type T.
        *
        * @throw DLLExceptions If the function pointer cannot be retrieved.
        */
        template<typename T, typename... Args>
        std::unique_ptr<T> getInstance(const std::string &entryPointName = "entryPoint", Args&&... args) {
            using EntryPointFunc = T *(*)(Args...);
            EntryPointFunc entryPoint = getEntryPoint<EntryPointFunc>(entryPointName);

            return std::unique_ptr<T>(entryPoint(std::forward<Args>(args)...));
        };

        /**
        * @class DLLExceptions
        *
        * @brief A custom exception class for errors related to dynamic library loading.
        */
        class DLLExceptions : public std::exception {
            public:
                DLLExceptions(const std::string &error) : _errMsg(error) {}

                const char *what() const noexcept override { return _errMsg.c_str(); }

            private:
                std::string _errMsg = "Unexpected error occurred";
        };

    private:
    #ifdef _WIN32
        HMODULE __library;
    #else
        void *__library;
    #endif
        /**
        * @brief Opens the specified library.
        *
        * @param libName The name of the shared library to open.
        *
        * @throw DLLExceptions If the library cannot be opened.
        */
        void openLibrary(const std::string &libPath, const std::string &libName) {
            std::string localLibName;
            std::string localLibPath;
            #ifdef _WIN32
                localLibName = libName + ".dll";
                localLibPath = libPath + localLibName;
                __library = LoadLibraryA(localLibPath.c_str());
                if (!__library) throw DLLExceptions("Failed to load library: " + localLibName);
            #else
                localLibName = "lib" + libName + ".so";
                localLibPath = libPath + localLibName;
                __library = dlopen(localLibPath.c_str(), RTLD_LAZY);
                if (!__library) throw DLLExceptions(dlerror());
            #endif
        };

        /**
        * @brief Closes the currently opened library.
        *
        * @throw std::exception If an error occurs during closing the library.
        */
        void closeLibrary() {
            #ifdef _WIN32
                if (!FreeLibrary((HMODULE)__library))
                    std::cerr << "Error while closing the library." << std::endl;
            #else
                if (dlclose(__library) != 0)
                    std::cerr << dlerror() << std::endl;
            #endif
        };

        /**
        * @brief Retrieves a function pointer from the library.
        *
        * @tparam T The type of the function pointer.
        *
        * @param entryPointName The name of the function to retrieve from the library.
        *
        * @return The function pointer.
        *
        * @throw DLLExceptions If the function pointer cannot be retrieved.
        */
        template<typename T>
        T getEntryPoint(const std::string &entryPointName) {

        #ifdef _WIN32
            T entryPoint = reinterpret_cast<T>(GetProcAddress(static_cast<HMODULE>(__library), entryPointName.c_str()));
        #else
            T entryPoint = reinterpret_cast<T>(dlsym(__library, entryPointName.c_str()));
        #endif

            if (!entryPoint) {
            #ifdef _WIN32
                throw DLLExceptions("Failed to load library");
            #else
                throw DLLExceptions(dlerror());
            #endif
            }
            return entryPoint;
        }
};

#endif /* !DLLOADER_HPP_ */
