cmake_minimum_required(VERSION 3.15)


add_library(FractionLib STATIC code/Fraction.cpp)

target_include_directories(FractionLib PUBLIC include)