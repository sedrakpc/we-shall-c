cmake_minimum_required(VERSION 3.23)
project(codecrafters-shell-c C)

set(CMAKE_C_STANDARD 99)

add_executable(codecrafters-shell-c app/parse_args.c app/array_list.c app/command.c app/main.c)

target_include_directories(codecrafters-shell-c PRIVATE include)
