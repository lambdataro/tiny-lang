cmake_minimum_required(VERSION 3.14)
project(tiny-lang)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Wextra")

add_executable(TinyLang Main.c Util.c Util.h Token.c Token.h Lexing.c Lexing.h Stream.c Stream.h StringBuffer.c StringBuffer.h Ast.c Ast.h Parsing.c Parsing.h Compile.c Compile.h)
