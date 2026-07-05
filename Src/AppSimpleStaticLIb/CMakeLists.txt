# Создаем исполняемый файл
add_executable(AppSimpleStaticLIb code/main.cpp)

# Линкуем приложение со статической библиотекой.
# CMake сам настроит зависимости и порядок сборки.
target_link_libraries(AppSimpleStaticLIb PRIVATE Greeter_lib)