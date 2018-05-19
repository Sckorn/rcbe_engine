cmake_minimum_required(VERSION 3.10)
project(rcbe_engine)
add_definitions(-std=c++17)

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
)

add_executable(
  ${PROJECT_NAME}
  src/main.cpp
  include/config/GlobalEngineConfig.h
  include/engine/Engine.h
  src/config/GlobalEngineConfig.cpp
  include/config/ConfigUtils.h
  include/data_types/gui_types.h
  include/gui/Window.h
  src/gui/Window.cpp
  include/gui/window_utils.h src/gui/window_utils.cpp include/config/ToolkitGUIConfig.h src/config/ToolkitGUIConfig.cpp include/config/config.h include/common/utils/file_utils.h)

target_link_libraries(
  ${PROJECT_NAME}
  ${Boost_LIBRARIES}
  ${CGAL_LIBRARIES}
  ${GTKMM_LIBRARIES}
)