cmake_minimum_required(VERSION 3.10)
project(rcbe_engine)
add_definitions(-std=c++17)

find_package(
  Boost REQUIRED COMPONENTS
  program_options
  filesystem
)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

find_package(
  CGAL REQUIRED
)

include(${CGAL_USE_FILE})


add_definitions(${GTK3_CFLAGS_OTHER})

include_directories(
  include
  ${Boost_INCLUDE_DIRS}
  ${CGAL_INCLUDE_DIRS}
  ${GTK3_INCLUDE_DIRS}
)

add_executable(
  ${PROJECT_NAME}
  src/main.cpp
  include/config/GlobalEngineConfig.h
  include/engine/Engine.h
  src/config/GlobalEngineConfig.cpp
  include/config/ConfigUtils.h
  include/data_types/gui_types.h include/gui/Window.h src/gui/Window.cpp)

target_link_libraries(
  ${PROJECT_NAME}
  ${Boost_LIBRARIES}
  ${CGAL_LIBRARIES}
  ${GTK3_LIBRARIES}
)