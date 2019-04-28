cmake_minimum_required(VERSION 3.14)
project(tiny-lang)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Wextra")

add_executable(TinyLang Main.c)

enable_testing()

add_test(Test01 TinyLang)
set_tests_properties(Test01 PROPERTIES
        PASS_REGULAR_EXPRESSION "TinyLang")
