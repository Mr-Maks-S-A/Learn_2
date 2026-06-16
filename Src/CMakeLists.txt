# 1. Создаем Header-Only (интерфейсную) библиотеку для наших шаблонов
add_library(utils INTERFACE)

# Указываем, где искать заголовки. 
# INTERFACE означает, что любой, кто подключит эту библиотеку, автоматически получит этот путь.
target_include_directories(utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 2. Создаем исполняемый файл калькулятора
add_executable(Preprocessor_directives   ${CMAKE_CURRENT_SOURCE_DIR}/code/main.cpp )

# Линкуем наши шаблоны к приложению
target_link_libraries(Preprocessor_directives PRIVATE utils)