# Plik utworzony przez menad?era projekt�w kdevelopa
# ------------------------------------------- 
# Podkatalog wzgl?dem g?�wnego katalogu projektu: ./src/gui
# Cel to biblioteka gui

HEADERS += mainwindow.h \
           playerdialog.h 
FORMS   += playerdialogbase.ui	   
SOURCES += mainwindow.cpp \
           playerdialog.cpp 
TARGET = gui
CONFIG += release warn_on qt staticlib
TEMPLATE = lib
QT += qt3support
INCLUDEPATH += ../compatibility \
	    ../database
