cmake_minimum_required(VERSION 3.15)
project(GreeterProject VERSION 1.0 LANGUAGES CXX)

# Задаем стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Настройка вывода бинарников (всё будет лежать в build/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Сюда уйдут .exe и .dll
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Сюда уйдут .so (в Linux)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # Сюда уйдут статические .lib/.a и файлы импорта

# Подключаем подпроекты
add_subdirectory(Src/Leaver_lib)
add_subdirectory(Src/AppSimpleStaticLIb)