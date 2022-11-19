cmake_minimum_required(VERSION 3.19)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13)

project(chessx
  VERSION "1.5.7"
  DESCRIPTION "A free chess database"
  HOMEPAGE_URL "https://chessx.sourceforge.io/"
  LANGUAGES CXX
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_SOUNDS "Enable sounds (requires Qt::Multimedia)" ON)
option(ENABLE_TTS "Enable text-to-speech (requires Qt::TextToSpeech)" ON)
option(ENABLE_SCID_SUPPORT "Enable support for Scid database format (*.si4)" ON)

# common definitions to use with Qt
add_library(qt_config INTERFACE)
target_compile_definitions(qt_config INTERFACE
  QT_DEPRECATED_WARNINGS
  QT_NO_CAST_TO_ASCII
  QT_USE_QSTRINGBUILDER

  # TODO: enable warnings
  # CONFIG += warn_on
)

# Find Qt
set(REQUIRED_QT_COMPONENTS
  LinguistTools
  Core
  Xml
  Test
  DBus
  Network
  OpenGL
  Svg
  PrintSupport
  Gui
  Widgets
)

if (ENABLE_SOUNDS)
  list(APPEND REQUIRED_QT_COMPONENTS Multimedia MultimediaWidgets)
endif()

if (ENABLE_TTS)
  list(APPEND REQUIRED_QT_COMPONENTS TextToSpeech)
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS "${REQUIRED_QT_COMPONENTS}")

add_subdirectory(dep)
add_subdirectory(src)

set(APP_NAME ${CMAKE_PROJECT_NAME})

set(TS_FILES
  # i18n/chessx_da.ts
  i18n/chessx_de.ts
  # i18n/chessx_fr.ts
  # i18n/chessx_it.ts
  # i18n/chessx_cz.ts
  # i18n/chessx_ru.ts
)

set(APP_SOURCES
  src/gui/main.cpp
  resources.qrc
  ${TS_FILES}

  data/images/chessx.icns
)

set(OTHER_FILES
  #[[
  data/templates/pgn-default.template
  data/templates/notation-default.template
  data/templates/latex-default.template
  data/templates/html-default.template
  ChangeLog
  COPYING
  ChangeLog.txt
  data/help/about.css
  data/help/about-dark.css
  data/help/about0.html
  data/help/about1.html
  data/help/about1a.html
  data/help/about2.html
  data/help/about3.html
  data/help/about4.html
  data/help/about5.html
  data/help/about6.html
  setup7-64.iss
  setup7-32.iss
  setupXP.iss
  data/styles/orange.css
  unix/chessx.desktop
  ]]
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(${APP_NAME}
      MANUAL_FINALIZATION
      ${APP_SOURCES}
  )
else()
  add_executable(${APP_NAME}
    ${APP_SOURCES}
  )
endif()

qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR}/src ${TS_FILES})

target_link_libraries(${APP_NAME} PRIVATE
  qt_config
  quazip
  bitboard
  board
  eco
  gui
)

set_target_properties(${APP_NAME} PROPERTIES
  MACOSX_BUNDLE TRUE

  # Custom template for Info.plist
  MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/pack/Info.plist.in

  # Configure content for Info.plist template
  MACOSX_BUNDLE_INFO_STRING "ChessX for Mac OS 10.14"
  MACOSX_BUNDLE_ICON_FILE chessx.icns
  MACOSX_BUNDLE_GUI_IDENTIFIER net.sourceforge.chessx
  MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
  MACOSX_BUNDLE_COPYRIGHT "(C) 2005-2022 ChessX developers"

  # Bundle resources
  RESOURCE data/images/chessx.icns

  WIN32_EXECUTABLE TRUE
)

if (APPLE)
  # Install the app
  install(
    TARGETS ${APP_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
  # Use macdeployqt on the installed app bundle
  find_program(MACDEPLOYQT macdeployqt
    PATHS ENV "Qt${QT_VERSION_MAJOR}_DIR"
    PATH_SUFFIXES bin
    REQUIRED
    NO_SYSTEM_ENVIRONMENT_PATH
  )
  string(CONFIGURE [==[
    execute_process(COMMAND @MACDEPLOYQT@ ${CMAKE_INSTALL_PREFIX}/@APP_NAME@.app)
  ]==] DEPLOY_SCRIPT @ONLY)
  install(CODE ${DEPLOY_SCRIPT})
else()
  install(TARGETS ${APP_NAME}
    RUNTIME DESTINATION bin
  )
endif()

if(QT_VERSION_MAJOR GREATER_EQUAL 6)
  qt_finalize_executable(${APP_NAME})
endif()

set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC OFF)

option(ENABLE_TESTING "Enable testing" OFF)
if (ENABLE_TESTING)
  enable_testing()
  add_subdirectory(tests/unittests)
endif()

add_subdirectory(pack)