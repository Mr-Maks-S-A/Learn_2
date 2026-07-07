#pragma once
#include <string>

// Окно экспорта/импорта для Windows. 
// Если собирается библиотека, макрос leaver_lib_EXPORTS определяется CMake автоматически.
#ifdef _WIN32
    #ifdef Leaver_lib_EXPORTS
        #define LEAVER_API __declspec(dllexport)
    #else
        #define LEAVER_API __declspec(dllimport)
    #endif
#else
    #define LEAVER_API // В Linux/macOS по умолчанию экспортируется всё
#endif

class LEAVER_API Leaver {
public:
    std::string leave(const std::string& name);
};
