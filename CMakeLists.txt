cmake_minimum_required(VERSION 3.16)
project(new_control_flow C)

set(CMAKE_C_STANDARD 99)

include_directories(./include)

add_executable(new_control_flow src/main.c include/error.h src/read_input.c include/file.h include/control_flow.h src/parser.c src/block.c src/subblock.c src/label.c src/utils.c include/utils.h include/pattern.h src/pattern.c src/dump_models.c include/dot.h src/dot/dot_blocks.c src/dot/dot_links.c src/old_path.c include/old_path.h src/dot/old_dot_block.s.c src/optimize_tree.c)