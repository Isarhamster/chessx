# Plik utworzony przez mened?era projekt� kdevelopa
# ------------------------------------------- 
# Podkatalog wzgl?dem g?�nego katalogu projektu: ./src/gui
# Cel to biblioteka gui

#The following line was changed from FORMS to FORMS3 by qt3to4
FORMS += playerdialog.ui \
         preferences.ui \
         boardsetup.ui \
         savedialog.ui \
         tipoftheday.ui
HEADERS += mainwindow.h \
           playerdialog.h \
           preferences.h \
           helpwindow.h \
           boardview.h \
           boardtheme.h \
           boardsetup.h \
           savedialog.h \
           gamelist.h \
	   chessbrowser.h \
           tipoftheday.h
SOURCES += mainwindow.cpp \
           playerdialog.cpp \
           preferences.cpp \
           boardview.cpp \
           boardtheme.cpp \
           boardsetup.cpp \
           helpwindow.cpp \
           savedialog.cpp \
           enginesetup.cpp \
           gamelist.cpp \
	   chessbrowser.cpp \
           tipoftheday.cpp
TARGET = gui
CONFIG += warn_on qt staticlib uic
TEMPLATE = lib
QT += qt3support
INCLUDEPATH += ../compatibility \
../database

RESOURCES     += helpwindow.qrc \
                 tipoftheday.qrc
