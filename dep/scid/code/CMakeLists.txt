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

cmake_minimum_required(VERSION 3.9)
project(scid)
set(CPACK_PACKAGE_VERSION 4.7.1)
set(
  CPACK_PACKAGE_DESCRIPTION_SUMMARY
  "chess database application with play and training functionality"
)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "tk8.6 (>= 8.6.0)")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
include(CPack)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17 CACHE STRING "")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CXX_FLAGS)
  set(CMAKE_BUILD_TYPE "Release")
endif()

if(MSVC)
  add_definitions(/D_CRT_SECURE_NO_WARNINGS /D_SCL_SECURE_NO_WARNINGS)
  string(APPEND CMAKE_CXX_FLAGS_RELEASE " /MT")
  # To run/debug using Visual Studio set "scid" as startup project and add:
  # Command Arguments: ../tcl/start.tcl
  # Environment:       PATH=C:\tcl\bin
endif()


# polyglot
file(GLOB POLYGLOT_SRC src/polyglot/*.cpp)
add_library(polyglot ${POLYGLOT_SRC})

# scid
file(GLOB SCID_SRC src/*.h src/*.cpp)
if(MSVC)
  add_executable(scid WIN32 ${SCID_SRC})
  target_link_options(scid PRIVATE /ENTRY:mainCRTStartup)
else()
  add_executable(scid ${SCID_SRC})
endif()
set_property(TARGET scid PROPERTY INTERPROCEDURAL_OPTIMIZATION True)


option(SCID_MULTITHREADING "Enable/disable multihreading" ON)
if(SCID_MULTITHREADING)
  find_package(Threads REQUIRED)
  target_link_libraries(scid ${CMAKE_THREAD_LIBS_INIT})
else()
  target_compile_definitions(scid PRIVATE -DMULTITHREADING_OFF)
endif()

option(SCID_USE_TB "Enable Nalimov tablebases" OFF)
if(SCID_USE_TB)
  target_compile_definitions(scid PRIVATE -DSCID_USE_TB PRIVATE -DT41_INCLUDE)
endif()

option(SPELLCHKVALIDATE "Verify the integrity of spelling files" OFF)
if(SPELLCHKVALIDATE)
  target_compile_definitions(scid PRIVATE -DSPELLCHKVALIDATE)
endif()

find_package(TCL)
target_include_directories(scid PRIVATE ${TCL_INCLUDE_PATH})
target_link_libraries(scid polyglot ${TCL_LIBRARY})

install(TARGETS scid DESTINATION scid)
install(PROGRAMS shell_scid DESTINATION bin RENAME scid)
install(DIRECTORY bitmaps DESTINATION scid)
install(DIRECTORY bitmaps2 DESTINATION scid)
install(DIRECTORY books DESTINATION scid)
install(DIRECTORY html DESTINATION scid)
install(DIRECTORY img DESTINATION scid)
install(DIRECTORY scripts DESTINATION scid)
install(DIRECTORY sounds DESTINATION scid)
install(DIRECTORY tcl DESTINATION scid)


# engine phalanx
file(GLOB PHALANX_SRC engines/phalanx-scid/*.c)
add_executable(phalanx-scid ${PHALANX_SRC})
set_target_properties(phalanx-scid PROPERTIES COMPILE_FLAGS "-w")
install(TARGETS phalanx-scid DESTINATION bin)


# engine scidlet
file(GLOB SCIDLET_SRC engines/scidlet/*.cpp)
set(SCIDLET_EXTRA
  src/engine.cpp
  src/recog.cpp
  src/misc.cpp
  src/position.cpp
)
add_executable(scidlet ${SCIDLET_SRC} ${SCIDLET_EXTRA})
install(TARGETS scidlet DESTINATION bin)

option(GTEST "Build unit tests" OFF)
if(GTEST)
  add_subdirectory(gtest)
endif()
