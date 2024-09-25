/*
** EPITECH PROJECT, 2024
** B-CPP-500-NAN-5-1-rtype-thomas.cluseau
** File description:
** DLLoader
*/

#ifndef DLLOADER_HPP
    #define DLLOADER_HPP

    #include <dlfcn.h>

class DLLoader {
    public:
        /**
        * @brief Constructs a DLLoader object and opens the specified library.
        *
        * @param libName The name of the shared library to open.
        *
        * @throw DLLExceptions If the library cannot be opened.
        */
        DLLoader(const std::string &libName);

        /**
        * @brief Destroys the DLLoader object and closes the currently opened library.
        *
        * @throw std::exception If an error occurs during closing the library.
        */
        ~DLLoader();

        /**
        * @brief Closes the currently opened library and opens a new library.
        *
        * @param libName The name of the new shared library to open.
        *
        * @throw DLLExceptions If the new library cannot be opened.
        */
        void loadNew(const std::string &libName);

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
        T *getInstance(const std::string &entryPointName = "entryPoint", Args&&... args) {
            using EntryPointFunc = T* (*)(Args...);
            EntryPointFunc entryPoint = reinterpret_cast<EntryPointFunc>(dlsym(__library, entryPointName.c_str()));

            if (!entryPoint) throw DLLExceptions(dlerror());

            return entryPoint(std::forward<Args>(args)...);
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
        void *__library;
};

#endif /* !DLLOADER_HPP_ */
