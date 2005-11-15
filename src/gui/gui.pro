# Plik utworzony przez menad?era projektów kdevelopa
# ------------------------------------------- 
# Podkatalog wzgl?dem g?ównego katalogu projektu: ./src/gui
# Cel to biblioteka gui

FORMS += playerdialogbase.ui \
         preferencesbase.ui
HEADERS += mainwindow.h \
           playerdialog.h \
           settings.h \
           preferences.h \
           helpwindow.h \
           boardview.h \
           boardtheme.h
SOURCES += mainwindow.cpp \
           playerdialog.cpp \
           settings.cpp \
           preferences.cpp \
           helpwindow.cpp \
           boardview.cpp \ 
           boardtheme.cpp 
TARGET = gui
CONFIG += release warn_on qt staticlib
TEMPLATE = lib
QT += qt3support
INCLUDEPATH += ../compatibility \
../database
