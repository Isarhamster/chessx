add_library(doctest_main STATIC doctest_main.cpp)
target_link_libraries(doctest_main PUBLIC doctest)

configure_file(resourcepath.h.in resourcepath.h)

add_executable(unittests
  ${CMAKE_CURRENT_BINARY_DIR}/resourcepath.h
  # unittests/test_indexitem.cpp
  unittests/test_index.cpp
)

target_include_directories(unittests PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(unittests
  PRIVATE
    doctest_main
    database
    gui
)

add_test(NAME unittests COMMAND unittests)

