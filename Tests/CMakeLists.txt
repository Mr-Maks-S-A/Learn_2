# Создаем исполняемый файл для тестов
add_executable(calc_tests calc_test.cpp)

# Подключаем наши шаблоны из Src к тестам. 
# Благодаря INTERFACE в Src, тесты теперь увидят и <Calc/Calculator.hpp>, и <Until/Input.hpp>
target_link_libraries(calc_tests PRIVATE calc_utils)


add_executable(input_tests input_test.cpp)
target_link_libraries(input_tests PRIVATE calc_utils)


# По желанию: включаем CTest, чтобы тесты можно было запускать командой `ctest`
enable_testing()
add_test(NAME RunParserTests COMMAND calc_tests)
add_test(NAME RunInputTests  COMMAND input_tests)
