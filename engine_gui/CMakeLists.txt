cmake_minimum_required(VERSION 3.10)
project(rcbe_engine_gui)
add_definitions(-std=c++17)

find_package(Threads)

find_package(
  Boost REQUIRED COMPONENTS
  program_options
  filesystem
)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

find_package(
  CGAL REQUIRED
)

include(${CGAL_USE_FILE})

include_directories(
  include
  ${Boost_INCLUDE_DIRS}
  ${CGAL_INCLUDE_DIRS}
  ${GTKMM_INCLUDE_DIRS}
  ../thirdparty/include/ #remove later when done with header only libs
  ../engine_datamodel/include/
  rcbe_engine_common
)

add_executable(
  ${PROJECT_NAME}
  src/main.cpp
  include/config/GlobalEngineConfig.h
  include/engine/Engine.h
  src/config/GlobalEngineConfig.cpp
  include/config/ConfigUtils.h
  include/gui/Window.h
  src/gui/Window.cpp
  include/gui/window_utils.h
  src/gui/window_utils.cpp
  include/config/WindowManagerConfig.h
  src/config/WindowManagerConfig.cpp
  include/config/config.h
  include/gui/WindowManager.h
  src/gui/WindowManager.cpp
  ../thirdparty/include/json/json.h
  ../engine_datamodel/include/data_types/gui_types.h
)

target_link_libraries(
  ${PROJECT_NAME}
  ${Boost_LIBRARIES}
  ${CGAL_LIBRARIES}
  ${GTKMM_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  #thirdparty
  rcbe_engine_common
)