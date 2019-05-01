cmake_minimum_required(VERSION 3.10)
project(rcbe_engine_datamodel)
add_definitions(-std=c++17)

add_library(
    ${PROJECT_NAME} 
    SHARED
    include/data_types/core_types.hpp
    include/data_types/gui_types.hpp
    include/data_types/rendering_types.hpp

    src/gui_types.cpp
    src/rendering_types.cpp
    )

include_directories(
  include
  ${Boost_INCLUDE_DIRS}
)

target_include_directories(
  ${PROJECT_NAME} INTERFACE include
)

set_target_properties(
    ${PROJECT_NAME} 
    PROPERTIES PUBLIC_HEADER 
    "include/data_types/core_types.hpp;include/data_types/gui_types.hpp;include/data_types/rendering_types.hpp")

install(
  TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION /usr/lib/x86_64-linux-gnu/
  PUBLIC_HEADER DESTINATION /usr/include/x86_64-linux-gnu/${PROJECT_NAME}
)


