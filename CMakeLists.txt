cmake_minimum_required(VERSION 4.0)
project(Worc-2_7_Simple_exception VERSION 2.7.1 LANGUAGES CXX)

# --- 1. Глобальные настройки проекта ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

# Настройка вывода бинарников (всё будет лежать в build/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


# --- 2. Подключение поддиректорий ---
add_subdirectory(Src)