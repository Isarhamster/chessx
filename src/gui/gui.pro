# Plik utworzony przez menad?era projektów kdevelopa
# ------------------------------------------- 
# Podkatalog wzgl?dem g?ównego katalogu projektu: ./src/gui
# Cel to biblioteka gui

FORMS   += playerdialogbase.ui	   
HEADERS += mainwindow.h \
           playerdialog.h 
SOURCES += mainwindow.cpp \
           playerdialog.cpp 
TARGET = gui
CONFIG += release warn_on qt staticlib
TEMPLATE = lib
QT += qt3support
INCLUDEPATH += ../compatibility \
	    ../database
