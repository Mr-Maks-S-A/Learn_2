cmake_minimum_required(VERSION 3.20)
project(Worc-2_1_Months VERSION 2.1.1 LANGUAGES CXX)

# --- 1. Глобальные настройки проекта ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройка вывода бинарников (чтобы всё лежало в одной папке build/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


set(CodeSorce ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)


add_executable(${PROJECT_NAME} ${CodeSorce})
