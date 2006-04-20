# Plik utworzony przez menad?era projektów kdevelopa
# ------------------------------------------- 
# Podkatalog wzgl?dem g?ównego katalogu projektu: ./src/gui
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
	   enginesetup.h
SOURCES += mainwindow.cpp \
           playerdialog.cpp \
           preferences.cpp \
           boardview.cpp \ 
           boardtheme.cpp \
           boardsetup.cpp \
           helpwindow.cpp \
	   savedialog.cpp \
	   enginesetup.cpp 
TARGET = gui
CONFIG +=  warn_on qt staticlib
TEMPLATE = lib
QT += qt3support
INCLUDEPATH += ../compatibility \
../database
