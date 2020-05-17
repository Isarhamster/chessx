add_executable(unittests
  doctest_main.cpp
)

target_link_libraries(unittests
  PUBLIC
    doctest
)
