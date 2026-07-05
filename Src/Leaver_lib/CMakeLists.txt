# Создаем статическую библиотеку с именем Leaver_lib
add_library(Leaver_lib SHARED 
    code/Leaver.cpp 
    include/Leaver.hpp
)

# Указываем, где лежат заголовочные файлы для тех, кто будет линковаться с этой либой.
# PUBLIC означает, что путь автоматически пробросится в проект приложения.
target_include_directories(Leaver_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)