cmake_minimum_required(VERSION 3.15)

#Первый тест
add_executable(FractionTests_1 Test_1.cpp)
target_link_libraries(FractionTests_1 PRIVATE FractionLib)
add_test(NAME Run_Basic_Tests COMMAND FractionTests_1)

#Второй максимальный тест
add_executable(FractionTests_2 Test_2.cpp)
target_link_libraries(FractionTests_2 PRIVATE FractionLib)
add_test(NAME Run_Stress_Tests COMMAND FractionTests_2)