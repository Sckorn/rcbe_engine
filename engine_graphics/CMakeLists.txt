cmake_minimum_required(VERSION 3.10)
project(rcbe_engine_graphics)
add_definitions(-std=c++17)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

find_package(
  Boost REQUIRED COMPONENTS
  filesystem
  #asio
)

include_directories(
  include
  ${Boost_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIRS}  
  ${GLUT_INCLUDE_DIRS}
)

add_library(
  ${PROJECT_NAME}
  SHARED
  include/core/constants.hpp
  include/core/types.hpp
  src/types.cpp
)

target_link_libraries(
  ${PROJECT_NAME}
  ${Boost_LIBRARIES}
  ${OPENGL_LIBRARIES} 
  ${GLUT_LIBRARY}
)

add_executable(
  ${PROJECT_NAME}_test_node
  src/core_test_node.cpp
)

target_link_libraries(
  ${PROJECT_NAME}_test_node
  ${PROJECT_NAME}
  ${Boost_LIBRARIES}
  ${OPENGL_LIBRARIES} 
  ${GLUT_LIBRARY}
)

target_include_directories(
  ${PROJECT_NAME} INTERFACE include/
)

set_target_properties(
  ${PROJECT_NAME} 
  PROPERTIES PUBLIC_HEADER 
  "include/core/constants.hpp;include/core/types.hpp"
  )

install(
  TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION /usr/lib/x86_64-linux-gnu/
  PUBLIC_HEADER DESTINATION /usr/include/x86_64-linux-gnu/${PROJECT_NAME}
)