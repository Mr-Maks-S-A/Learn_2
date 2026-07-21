

# *.cpp — Стандартный файл исходного кода C++ (C Plus Plus).
# *.cxx — Альтернативное расширение для исходного кода C++.
# *.c — Файл исходного кода на языке C.
# *.hpp — Стандартный заголовочный файл C++, содержащий объявления и шаблоны.
# *.hxx — Альтернативное расширение для заголовочных файлов C++.
# *.inl — Файл с реализацией inline-функций и шаблонов для выноса из заголовков (только для подключения в заголовках не в ресурсных файлах).
# *.h — Классический заголовочный файл C/C++ для объявлений функций и структур.
# Собираем все .cpp и .h файлы из папки src и include в переменную
file(GLOB COMMON_CORE_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Core/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Core/*.hxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Core/*.inl"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Core/*.h"
)


add_library(CommonCoreLib STATIC ${COMMON_CORE_SOURCES})
set_target_properties(CommonCoreLib PROPERTIES LINKER_LANGUAGE CXX)

add_library(RacingProject::CommonCoreLib ALIAS CommonCoreLib)

target_include_directories(CommonCoreLib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Добавляем прекомпилированный заголовок к цели
target_link_libraries(CommonCoreLib PUBLIC RacingProject::SharedPCH)