cmake_minimum_required(VERSION 3.14)
project(tiny-lang)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Wextra")

add_executable(TinyLang Main.c Stream.c Stream.h Util.c Util.h StringBuffer.c StringBuffer.h Token.c Token.h Lexing.c Lexing.h Ast.c Ast.h Parsing.c Parsing.h Eval.c Eval.h Value.c Value.h SymbolTable.c SymbolTable.h)

enable_testing()

function(do_test name result)
    add_test(${name} TinyLang "${CMAKE_SOURCE_DIR}/tests/${name}.tl")
    set_tests_properties(${name} PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endfunction(do_test)

do_test(test01 "11\n+$")
do_test(test02 "190\n+$")
do_test(test03 "6\n5\n+$")
do_test(test04 "6\n+$")
do_test(test05 "1\n2\n+$")
do_test(test06 "55\n+$")
do_test(test07 "100\n+$")
