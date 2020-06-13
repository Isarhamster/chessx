
configure_file(resourcepath.h.in resourcepath.h)

set(COMMON_DEPENDENCIES database eco)

add_executable(doctestrunner
  doctest_main.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/resourcepath.h

  test_index.cpp
  test_integralmetrics.cpp
  test_resultscounter.cpp
)

target_include_directories(doctestrunner PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(doctestrunner PRIVATE doctest ${COMMON_DEPENDENCIES})
add_test(NAME unit.doctest COMMAND doctestrunner)

#
# Qt Test unit tests
#

function(define_qttest_test test_name exec_name)
  add_executable(${exec_name}
    ${CMAKE_CURRENT_BINARY_DIR}/resourcepath.h
  )
  set_property(TARGET ${exec_name} PROPERTY AUTOMOC ON)

  set(includes)
  set(fixtures)
  foreach(fixture ${ARGN})
    string(TOLOWER ${fixture} fixture_lower)
    # source files base name
    set(base_name ${fixture_lower}test)
    # name of test fixture class
    set(type_name ${fixture}Test)

    target_sources(${exec_name} PRIVATE ${base_name}.cpp ${base_name}.h)

    string(APPEND includes "#include \"${base_name}.h\"\n")
    string(APPEND fixtures "    FIXTURE(${type_name});\n")
  endforeach()

  configure_file(qttestmain.cpp.in ${exec_name}.cpp)
  target_sources(${exec_name} PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/${exec_name}.cpp
  )

  target_include_directories(${exec_name}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
  )
  target_link_libraries(${exec_name} PRIVATE Qt5::Test ${COMMON_DEPENDENCIES})

  add_test(NAME ${test_name} COMMAND ${exec_name})
endfunction()

define_qttest_test(unit.qttest qttestrunner
  Board
  DatabaseConversion
  Game
  PgnDatabase
  PlayerDatabase
  PositionSearch
  SpellChecker
)

