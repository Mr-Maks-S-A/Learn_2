# Создаем заголовочную цель для PCH
add_library(SharedPCH INTERFACE)

add_library(RacingProject::SharedPCH ALIAS SharedPCH)

# Указываем путь к нашему единому pch.h
target_precompile_headers(SharedPCH INTERFACE 
    "${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h"
)