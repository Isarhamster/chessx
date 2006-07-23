# Plik utworzony przez mened?era projekt�w kdevelopa
# ------------------------------------------- 
# Podkatalog wzgl?dem g?�wnego katalogu projektu: ./src/gui
# Cel to biblioteka gui

FORMS += playerdialogbase.ui \
         preferencesbase.ui \
         boardsetupbase.ui \
         enginesetupbase.ui \
         savedialogbase.ui
HEADERS += mainwindow.h \
           playerdialog.h \
           preferences.h \
           helpwindow.h \
           boardview.h \
           boardtheme.h \
           boardsetup.h \
           savedialog.h \
           enginesetup.h \
           gamelist.h 
SOURCES += mainwindow.cpp \
           playerdialog.cpp \
           preferences.cpp \
           boardview.cpp \
           boardtheme.cpp \
           boardsetup.cpp \
           helpwindow.cpp \
           savedialog.cpp \
           enginesetup.cpp \
           gamelist.cpp 
TARGET = gui
CONFIG += warn_on qt staticlib
TEMPLATE = lib
QT += qt3support
INCLUDEPATH += ../compatibility \
../database
