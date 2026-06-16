# 1. Создаем Header-Only (интерфейсную) библиотеку для наших шаблонов
add_library(calc_utils INTERFACE)

# Указываем, где искать заголовки. 
# INTERFACE означает, что любой, кто подключит эту библиотеку, автоматически получит этот путь.
target_include_directories(calc_utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 2. Создаем исполняемый файл калькулятора
add_executable(calculator_app ${CMAKE_CURRENT_SOURCE_DIR}/code/main.cpp)

# Линкуем наши шаблоны к приложению
target_link_libraries(calculator_app PRIVATE calc_utils)