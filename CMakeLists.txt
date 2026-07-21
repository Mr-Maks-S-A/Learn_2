cmake_minimum_required(VERSION 3.20)
project(RacingSimulator VERSION 1.0 LANGUAGES CXX)

# Автоматическое обнаружение и интеграция ccache.
# Если ccache установлен в системе, он перехватывает вызовы компилятора C++
# и кэширует скомпилированные объектные файлы (включая PCH), что ускоряет 
# повторную сборку (incremental build) и очистку (clean build) в десятки раз.
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    message(STATUS "ccache обнаружен и подключен для ускорения сборки")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


add_subdirectory("${CMAKE_SOURCE_DIR}/Internal Sub-projects/SupportPCH")
add_subdirectory("${CMAKE_SOURCE_DIR}/Internal Sub-projects/CommonCore_Lib")
# add_subdirectory("${CMAKE_SOURCE_DIR}/Internal Sub-projects/RacingEngine_DLL")
add_subdirectory("${CMAKE_SOURCE_DIR}/Internal Sub-projects/${PROJECT_NAME}_APP")


