cmake_minimum_required(VERSION 3.10)
project(rcbe_engine)
add_definitions(-std=c++17)

add_subdirectory(engine_common)
add_subdirectory(engine_datamodel)
add_subdirectory(engine_graphics)
add_subdirectory(engine_gui)