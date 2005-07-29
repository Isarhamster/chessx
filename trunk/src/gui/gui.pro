# Plik utworzony przez menad?era projektów kdevelopa
# ------------------------------------------- 
# Podkatalog wzgl?dem g?ównego katalogu projektu: ./src/gui
# Cel to biblioteka gui

HEADERS += mainwindow.h \
           $$QT34/qmainwindow.h \
           $$QT34/qpopupmenu.h \
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
QT34 = ../compatibility/qt34
