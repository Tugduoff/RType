#ifndef DLL_LOADER_HPP
    #define DLL_LOADER_HPP

    #include <dlfcn.h>
    #include <memory>
    #include <stdexcept>

template<typename T>
class DLLoader {
    class DLLoaderException : public std::exception {
        std::string message;

        public:
            DLLoaderException(const char* error) : message(error) {}
            const char* what() const noexcept override { return message.c_str(); }
    };

    public:
        /// @brief Loads the dynamic library specified by `PluginPath`.
        /// @param PluginPath The path to the library to load.
        /// @throws DLLoaderException If the library cannot be loaded.
        DLLoader(const std::string& PluginPath) : handle(nullptr) {
            handle = dlopen(PluginPath.c_str(), RTLD_LAZY);
            if (!handle) throw DLLoaderException(dlerror());
        }

        /// @brief Unloads the dynamic library if it has been loaded.   
        ~DLLoader() { if (handle) dlclose(handle); }

        /// @brief Gets an instance of the specified function in the library.
        /// Finds the symbol corresponding to `functionName` in the loaded library.
        /// @param functionName The name of the function to search for.
        /// @return A pointer to the function found.
        /// @throws DLLoaderException If the library is not loaded or if the function is not found.
        T* getInstance(const std::string& functionName) {
            if (!handle) throw DLLoaderException("Library not loaded");

            void* symbol = dlsym(handle, functionName.c_str());
            if (!symbol) throw DLLoaderException(dlerror());

            T* (*createFunction)() = reinterpret_cast<T* (*)()>(symbol);
            return createFunction();
        }

    private:
        void* handle;
};

#endif /* !DLL_LOADER_HPP_ */
