cmake_minimum_required(VERSION 4.0)
project(Worc-2_3_Addresses VERSION 2.3.1 LANGUAGES CXX)

# --- 1. Глобальные настройки проекта ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройка вывода бинарников (чтобы всё лежало в одной папке build/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


# Найти все исходные файлы (более явный способ)
# file(GLOB_RECURSE APP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c**" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h**")
# --- 2. Основное приложение ---
set(APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_executable(${PROJECT_NAME} ${APP_SOURCES})

# Общие инклюды для всего проекта (чтобы инклуды внутри include/ работали везде)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Создаем отдельный таргет для тестов
set(TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests.cpp) # Вынесем тесты в отдельный файл
add_executable(PerformanceTests ${TEST_SOURCES})

# Общие инклюды для всего проекта (чтобы инклуды внутри include/ работали везде)
target_include_directories(PerformanceTests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Копирование файла при каждой сборке (на случай изменений в исходнике)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/Sorce/in.txt"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Sorce/in.txt"
)