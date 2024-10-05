#ifndef LIBRARY_ENTRYPOINT_HPP
    #define LIBRARY_ENTRYPOINT_HPP


    #ifdef _WIN32
        #define WINDOWS_DLL_EXPORT __declspec(dllexport)
    #else
        #define WINDOWS_DLL_EXPORT
    #endif

    #define  LIBRARY_ENTRYPOINT extern "C" WINDOWS_DLL_EXPORT

#endif /* LIBRARY_ENTRYPOINT */
