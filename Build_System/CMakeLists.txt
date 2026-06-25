cmake_minimum_required(VERSION 3.15)
project(Worc-2_9_Cmake_simple_Project VERSION 2.9.1 LANGUAGES CXX)

# --- 1. Глобальные настройки проекта ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

# Настройка вывода бинарников (всё будет лежать в build/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Указываем путь к исходникам относительно папки build_system
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Src")

add_executable(Worc-2_9_Cmake_simple_Project
    "${SOURCE_DIR}/code/main.cpp"
)