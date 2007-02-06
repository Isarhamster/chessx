# Main application

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
           gamelist.cpp \
	   chessbrowser.cpp \
           tipoftheday.cpp \
	   main.cpp
TARGET = gui
TEMPLATE = app
INCLUDEPATH += ../database \
    ../compatibility
RESOURCES     += helpwindow.qrc \
                 tipoftheday.qrc
TARGET = ../../bin/chessdatabase
LIBS += ../database/libdatabase.a
PRE_TARGETDEPS = ../database/libdatabase.a

include(../common.pro)
CONFIG += uic
QT += qt3support


