
# *.cpp — Стандартный файл исходного кода C++ (C Plus Plus).
# *.cxx — Альтернативное расширение для исходного кода C++.
# *.c — Файл исходного кода на языке C.
# *.hpp — Стандартный заголовочный файл C++, содержащий объявления и шаблоны.
# *.hxx — Альтернативное расширение для заголовочных файлов C++.
# *.inl — Файл с реализацией inline-функций и шаблонов для выноса из заголовков (только для подключения в заголовках не в ресурсных файлах).
# *.h — Классический заголовочный файл C/C++ для объявлений функций и структур.
# Собираем исходники
file(GLOB APP_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.inl"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

# Имя исполняемого файла станет RacingSimulator_APP (или RacingSimulator.exe)
set(EXE_NAME "${PROJECT_NAME}_APP")

add_executable(${EXE_NAME} ${APP_SOURCES})

# Псевдоним тоже соберется красиво: RacingProject::RacingSimulator_APP
add_executable(RacingProject::${EXE_NAME} ALIAS ${EXE_NAME})

target_include_directories(${EXE_NAME} PRIVATE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(${EXE_NAME} PRIVATE 
    RacingProject::CommonCoreLib
#     RacingProject::RacingEngineDLL
)

# Добавляем прекомпилированный заголовок к цели
target_link_libraries(${EXE_NAME} PUBLIC RacingProject::SharedPCH)