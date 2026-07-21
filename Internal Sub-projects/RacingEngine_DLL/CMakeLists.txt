


# *.cpp — Стандартный файл исходного кода C++ (C Plus Plus).
# *.cxx — Альтернативное расширение для исходного кода C++.
# *.c — Файл исходного кода на языке C.
# *.hpp — Стандартный заголовочный файл C++, содержащий объявления и шаблоны.
# *.hxx — Альтернативное расширение для заголовочных файлов C++.
# *.inl — Файл с реализацией inline-функций и шаблонов для выноса из заголовков (только для подключения в заголовках не в ресурсных файлах).
# *.h — Классический заголовочный файл C/C++ для объявлений функций и структур.
# Собираем все .cpp и .h файлы из папки src и include в переменную
file(GLOB RACING_ENGINE_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    
    "${CMAKE_CURRENT_SOURCE_DIR}/include/RacingEngine/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/RacingEngine/*.hxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/RacingEngine/*.inl"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/RacingEngine/*.h"
)

add_library(RacingEngineDLL SHARED ${RACING_ENGINE_SOURCES})
set_target_properties(RacingEngineDLL PROPERTIES LINKER_LANGUAGE CXX)

# 3. Создаем псевдоним (ALIAS) с пространством имён
add_library(RacingProject::RacingEngineDLL ALIAS RacingEngineDLL)

target_include_directories(RacingEngineDLL PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_definitions(RacingEngineDLL PRIVATE RACING_ENGINE_EXPORTS)

# target_link_libraries(RacingEngineDLL PRIVATE RacingProject::CommonCoreLib)

# Добавляем прекомпилированный заголовок к цели
target_link_libraries(RacingEngineDLL PUBLIC RacingProject::SharedPCH)