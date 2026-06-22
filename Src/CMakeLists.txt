# 1. Создаем опцию в CMake (по умолчанию OFF, то есть "Попытка выстоять")
option(SHUTDOWN_ON_ERROR "Shutdown program immediately on figure error" OFF)

# 2. Если пользователь передал -DSHUTDOWN_ON_ERROR=ON (или любая истина),
# прокидываем #define SHUTDOWN_ON_ERROR в компилятор
if(SHUTDOWN_ON_ERROR)
    add_compile_definitions(SHUTDOWN_ON_ERROR)
    message(STATUS "🔥 Режим сборки: Срочная остановка (SHUTDOWN_ON_ERROR=ON)")
else()
    message(STATUS "🩹 Режим сборки: Попытка выстоять (SHUTDOWN_ON_ERROR=OFF)")
endif()



# 1. Создаем Header-Only (интерфейсную) библиотеку для наших шаблонов
add_library(utils INTERFACE)

# Указываем, где искать заголовки. 
# INTERFACE означает, что любой, кто подключит эту библиотеку, автоматически получит этот путь.
target_include_directories(utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 2. Создаем исполняемый файл калькулятора
add_executable(Class_constructor_exception   ${CMAKE_CURRENT_SOURCE_DIR}/code/main.cpp )

# Линкуем наши шаблоны к приложению
target_link_libraries(Class_constructor_exception PRIVATE utils)