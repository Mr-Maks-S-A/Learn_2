cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(ascy_renderer)

# --- 1. Глобальные настройки проекта ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

# Настройка вывода бинарников (всё будет лежать в build/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


set(GLM_TEST_ENABLE OFF CACHE INTERNAL "")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glm)

add_executable(${PROJECT_NAME}   ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp )

target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
