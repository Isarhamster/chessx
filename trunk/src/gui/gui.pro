# Plik utworzony przez menad?era projektów kdevelopa
# ------------------------------------------- 
# Podkatalog wzgl?dem g?ównego katalogu projektu: ./src/gui
# Cel to biblioteka gui

FORMS += playerdialogbase.ui \
         preferencesbase.ui \
	 boardsetupbase.ui
HEADERS += mainwindow.h \
           playerdialog.h \
           settings.h \
           preferences.h \
           helpwindow.h \
           boardview.h \
           boardtheme.h \
           boardsetup.h 
SOURCES += mainwindow.cpp \
           playerdialog.cpp \
           settings.cpp \
           preferences.cpp \
           boardview.cpp \ 
           boardtheme.cpp \
           boardsetup.cpp \
           helpwindow.cpp 
TARGET = gui
CONFIG += release warn_on qt staticlib
TEMPLATE = lib
QT += qt3support
INCLUDEPATH += ../compatibility \
../database
