cmake_minimum_required(VERSION 3.16)
project(kent_control_flow C)

set(CMAKE_C_STANDARD 99)

include_directories(./include)

add_executable(kent_control_flow src/main.c src/read_file.c src/dot/dot_dump.c src/parser.c src/block_controller.c)