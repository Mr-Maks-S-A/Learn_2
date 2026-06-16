add_executable(input_tests input_test.cpp)
target_link_libraries(input_tests PRIVATE utils)


# По желанию: включаем CTest, чтобы тесты можно было запускать командой `ctest`
enable_testing()
add_test(NAME RunInputTests  COMMAND input_tests)
