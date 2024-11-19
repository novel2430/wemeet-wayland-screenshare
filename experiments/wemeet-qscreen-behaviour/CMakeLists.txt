cmake_minimum_required(VERSION 3.26)
project(wemeet-qscreen-behaviour LANGUAGES C CXX)

find_package(Qt5 REQUIRED COMPONENTS Gui)

add_library(qscreen_hook SHARED qscreen_hook.cpp)
set_target_properties(qscreen_hook PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_link_libraries(qscreen_hook PRIVATE Qt5::Gui)


