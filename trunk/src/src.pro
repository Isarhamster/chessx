# Plik utworzony przez menad?era projektów kdevelopa
# ------------------------------------------- 
# Podkatalog wzgl?dem g?ównego katalogu projektu: ./src
# Cel to program:  ../bin/chessdatabase

SOURCES += main.cpp 
INCLUDEPATH += ./database ./gui
TEMPLATE = app
CONFIG += release warn_on thread qt
TARGET = ../bin/chessdatabase
QT += qt3support
INCLUDEPATH += compatibility
QT34 = compatibility/qt34
win32{
  release{
    LIBS += gui/release/libgui.a database/release/libdatabase.a
  }
  else{
    LIBS += gui/debug/libgui.a database/debug/libdatabase.a
  }
}
else{
  LIBS += gui/libgui.a database/libdatabase.a
}
