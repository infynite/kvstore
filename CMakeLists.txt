cmake_minimum_required(VERSION 3.10)
project(kvstore C)

set(CMAKE_C_STANDARD 11)

add_executable(kvstore main.c kvstore.h map.h dynamic.h syntax.h mem.h list.h str.h test_write_data.h)