cmake_minimum_required(VERSION 3.23)

project(wemeet-x11-behaviour LANGUAGES C CXX)

find_package(X11 REQUIRED)

function(make_hooklib FILENAME)
  # TARGET_NAME is FILENAME without extension
  get_filename_component(TARGET_NAME ${FILENAME} NAME_WE)
  add_library(${TARGET_NAME} SHARED ${FILENAME})
  set_target_properties(${TARGET_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
  target_link_libraries(${TARGET_NAME} PRIVATE X11::X11)
endfunction()

make_hooklib(1_basic_hook.cpp)
make_hooklib(2_xdamage_hook.cpp)