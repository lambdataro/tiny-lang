cmake_minimum_required(VERSION 3.14)
project(tiny-lang)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Wextra")

add_executable(TinyLang Main.c)

enable_testing()

function(do_test name result)
    add_test(${name} TinyLang "${CMAKE_SOURCE_DIR}/tests/${name}.tl")
    set_tests_properties(${name} PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endfunction(do_test)

do_test(test01 "11\n$")
do_test(test02 "190\n$")
