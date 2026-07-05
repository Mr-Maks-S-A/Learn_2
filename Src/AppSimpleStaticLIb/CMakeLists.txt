# Создаем исполняемый файл
add_executable(AppSimpleDLL_LIb code/main.cpp)

# Линкуем приложение со статической библиотекой.
# CMake сам настроит зависимости и порядок сборки.
target_link_libraries(AppSimpleDLL_LIb PRIVATE Leaver_lib)