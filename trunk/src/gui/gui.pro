# Plik utworzony przez menad?era projekt�w kdevelopa
# ------------------------------------------- 
# Podkatalog wzgl?dem g?�wnego katalogu projektu: ./src/gui
# Cel to biblioteka gui

FORMS   += playerdialogbase.ui	   
HEADERS += mainwindow.h \
           playerdialog.h \
	   settings.h
SOURCES += mainwindow.cpp \
           playerdialog.cpp \
	   settings.cpp 
TARGET = gui
CONFIG += release warn_on qt staticlib
TEMPLATE = lib
QT += qt3support
INCLUDEPATH += ../compatibility \
	    ../database
