cmake_minimum_required(VERSION 3.14)
project(tiny-lang)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Wextra")

add_executable(TinyLang Main.c Stream.c Stream.h Util.c Util.h Compiler.c Compiler.h Token.c Token.h Lexer.c Lexer.h StringBuffer.c StringBuffer.h Ast.c Ast.h Parser.c Parser.h)
