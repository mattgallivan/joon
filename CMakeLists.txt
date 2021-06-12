cmake_minimum_required (VERSION 3.14.1)
set(CMAKE_BUILD_TYPE Debug)

project (joon)
add_compile_options (-pedantic -Wall -Wextra -Werror -Wswitch-default -Wswitch-enum)

add_executable (joon src/joon.c src/lexer.c src/token.c src/bigint.c src/bigfloat.c)
target_include_directories(joon PRIVATE src)
