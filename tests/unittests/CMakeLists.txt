
configure_file(resourcepath.h.in resourcepath.h)

set(COMMON_DEPENDENCIES database eco)

add_executable(doctestrunner
  doctest_main.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/resourcepath.h

  test_index.cpp
)

target_include_directories(doctestrunner PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(doctestrunner PRIVATE doctest ${COMMON_DEPENDENCIES})
add_test(NAME unit.doctest COMMAND doctestrunner)

#
# Qt Test unit tests
#

# Creates a test for a single QtTest fixture class
function(qttest_fixture name)
  string(TOLOWER ${name} name_lower)
  # name of executable target
  set(exec_name test_${name_lower})
  # source files base name
  set(base_name ${name_lower}test)
  # name of test fixture class
  set(test_name ${name}Test)

  configure_file(qttestmain.cpp.in ${exec_name}.cpp)

  add_executable(${exec_name}
    ${CMAKE_CURRENT_BINARY_DIR}/${exec_name}.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/resourcepath.h
    ${base_name}.cpp
  )
  set_property(TARGET ${exec_name} PROPERTY AUTOMOC ON)
  target_include_directories(${exec_name}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
  )
  target_link_libraries(${exec_name} PRIVATE Qt5::Test ${COMMON_DEPENDENCIES})

  add_test(NAME QtTest.${name} COMMAND ${exec_name})
endfunction()

qttest_fixture(Board)

