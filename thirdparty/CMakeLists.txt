cmake_minimum_required(VERSION 3.10)
project(rcbe_engine_thirdparty)
add_definitions(-std=c++17)

#include_directories(
#  include
#)

add_library(
    ${PROJECT_NAME}
    INTERFACE
)

target_include_directories(${PROJECT_NAME} INTERFACE include/)

#install(
#    TARGETS
#    ${PROJECT_NAME}
#    DESTINATION
#    /usr/lib/x86_64-linux-gnu/
#)

#install(
#    TARGETS
#    ${PROJECT_NAME}
#    PUBLIC_HEADER
#    "include/json/json.h" DESTINATION /usr/include/
#)