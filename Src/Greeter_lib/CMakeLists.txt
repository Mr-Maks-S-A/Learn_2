# Создаем статическую библиотеку с именем Greeter_lib
add_library(Greeter_lib STATIC 
    code/Greeter.cpp 
    include/Greeter.hpp
)

# Указываем, где лежат заголовочные файлы для тех, кто будет линковаться с этой либой.
# PUBLIC означает, что путь автоматически пробросится в проект приложения.
target_include_directories(Greeter_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)