# Plik utworzony przez menad?era projektów kdevelopa
# ------------------------------------------- 
# Podkatalog wzgl?dem g?ównego katalogu projektu: ./src
# Cel to program:  ../bin/chessdatabase

HEADERS += database/common.h 
SOURCES += main.cpp 
INCLUDEPATH += ./database ./gui
TEMPLATE = app
CONFIG += release warn_on thread qt
TARGET = ../bin/chessdatabase

win32 {
  release {
    LIBS += gui/release/libgui.a database/release/libdatabase.a
  } else {
    LIBS += gui/debug/libgui.a database/debug/libdatabase.a
  }
} else {
  LIBS += gui/libgui.a database/libdatabase.a
}
                                                              
# for qt3/4 compatibility
QT += qt3support

INCLUDEPATH += compatibility
QT34 = compatibility/qt34
