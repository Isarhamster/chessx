# Copyright (C) 2016-2017  Fulvio Benini
# This file is part of Scid (Shane's Chess Information Database).
#
# Scid is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation.
#
# Scid is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Scid. If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 17 CACHE STRING "")

# googletest
if(NOT IS_DIRECTORY "${CMAKE_BINARY_DIR}/googletest")
  find_package(Git)
  execute_process(COMMAND ${GIT_EXECUTABLE} clone https://github.com/google/googletest.git)
endif()
set(gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll")
add_subdirectory(${CMAKE_BINARY_DIR}/googletest ${CMAKE_BINARY_DIR}/googletest/Build EXCLUDE_FROM_ALL)

# scid_sources
set(SCID_BASE
  ../src/codec_scid4.cpp
  ../src/scidbase.cpp
  ../src/sortcache.cpp
  ../src/stored.cpp
  ../src/game.cpp ../src/position.cpp ../src/textbuf.cpp ../src/misc.cpp
)
add_library(scid_base ${SCID_BASE})
target_include_directories(scid_base PUBLIC ../src)
if(SCID_MULTITHREADING)
  find_package(Threads REQUIRED)
  target_link_libraries(scid_base PUBLIC ${CMAKE_THREAD_LIBS_INIT})
else()
  target_compile_definitions(scid_base PUBLIC -DMULTITHREADING_OFF)
endif()

# scid_tests
file(GLOB GTEST_SRC *.cpp)
add_executable(scid_tests ${GTEST_SRC})
target_compile_definitions(scid_tests PRIVATE -DSCID_TESTDIR=\"${CMAKE_CURRENT_LIST_DIR}/\")
target_link_libraries(scid_tests PRIVATE scid_base gtest_main)
