cmake_minimum_required(VERSION 3.10)
project(rcbe_engine_common)
add_definitions(-std=c++17)

find_package(
  Boost REQUIRED COMPONENTS
  filesystem
)

include_directories(
  include
  ${Boost_INCLUDE_DIRS}
)

add_library(
  ${PROJECT_NAME}
  SHARED
  include/common/utils/file_utils.h
  src/common/utils/file_utils.cpp
)

target_link_libraries(
  ${PROJECT_NAME}
  ${Boost_LIBRARIES}
)

