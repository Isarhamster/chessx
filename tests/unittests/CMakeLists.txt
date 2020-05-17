
configure_file(resourcepath.h.in resourcepath.h)

add_executable(runner
  doctest_main.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/resourcepath.h

  test_index.cpp
)

target_include_directories(runner PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(runner
  PRIVATE
    doctest
    database
    gui
)

add_test(NAME unittests COMMAND runner)

