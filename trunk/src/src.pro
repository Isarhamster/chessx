# Plik utworzony przez menad?era projekt�w kdevelopa
# ------------------------------------------- 
# Podkatalog wzgl?dem g?�wnego katalogu projektu: ./src
# Cel to program:  ../bin/chessdatabase

HEADERS += database/common.h 
SOURCES += main.cpp 
INCLUDEPATH += ./database ./gui
LIBS += gui/libgui.a database/libdatabase.a
TEMPLATE = app
CONFIG += release warn_on thread qt
TARGET = ../bin/chessdatabase
