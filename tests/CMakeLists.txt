function(make_exe FILENAME)
  # target name is filename minus ".cpp"
  string(REGEX REPLACE ".cpp" "" TARGET_NAME ${FILENAME})
  add_executable(${TARGET_NAME} ${FILENAME})
  add_compile_options_for(${TARGET_NAME})
endfunction()

make_exe(framebuf_test.cpp)

make_exe(test_main.cpp)
target_link_libraries(test_main PRIVATE hook)