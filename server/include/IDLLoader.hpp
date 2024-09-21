#ifndef IDLL_LOADER_HPP
    #define IDLL_LOADER_HPP

    #include <dlfcn.h>
    #include <memory>
    #include <stdexcept>

class IDLLoader {
    public:
        /// @brief Loads the dynamic library specified by `PluginPath`.
        /// @param PluginPath The path to the library to load.
        /// @throws std::runtime_error If the library cannot be loaded.
        IDLLoader(const std::string& PluginPath) : handle(nullptr) {
            handle = dlopen(PluginPath.c_str(), RTLD_LAZY);
            if (!handle) throw std::runtime_error("Error loading Plugin: " + std::string(dlerror()));
        }

        /// @brief Unloads the dynamic library if it has been loaded.
        ~IDLLoader() { if (handle) dlclose(handle); }

        /// @brief Gets an instance of the specified function in the library.
        /// Finds the symbol corresponding to `functionName` in the loaded library.
        /// @param functionName The name of the function to search for.
        /// @return A pointer to the function found.
        /// @throws std::runtime_error If the library is not loaded or if the function is not found.
        template<typename T>
        T* getInstance(const std::string& functionName) {
            if (!handle) throw std::runtime_error("Plugin not loaded");

            void* symbol = dlsym(handle, functionName.c_str());
            if (!symbol) throw std::runtime_error("Error finding symbol: " + std::string(dlerror()));

            T* (*createFunction)() = reinterpret_cast<T* (*)()>(symbol);
            return createFunction();
        }

    private:
        void* handle;
};

#endif /* !IDLL_LOADER_HPP_ */
